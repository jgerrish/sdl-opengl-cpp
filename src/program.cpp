#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#include "program.h"
#include "shader.h"

using namespace sdl_opengl_cpp;

Program::Program(const string &program_name,
                 const std::shared_ptr<GLContext> &ctx,
                 deque<unique_ptr<Shader>> &shaders)
    : name{program_name}, gl_context{ctx} {
  if (shaders.size() > MAX_SHADERS) {
#ifndef NO_EXCEPTIONS
    spdlog::error("ERROR::SHADER::PROGRAM::CREATE_PROGRAM_FAILED::{}", name);
    throw ProgramCreationError("ERROR::SHADER::PROGRAM::CREATE_PROGRAM_FAILED");
#else
    set_error(std::optional<error>(error::ProgramCreationError));
    cleanup();
    return;
#endif
  }

  // spdlog::info("Linking OpenGL program");

  program = gl_context->glCreateProgram();

  GLenum error = gl_context->glGetError();

  if ((error == GL_OUT_OF_MEMORY) || (program == 0)) {
#ifndef NO_EXCEPTIONS
    spdlog::error("ERROR::SHADER::PROGRAM::CREATE_PROGRAM_FAILED::{}", name);
    throw ProgramCreationError("ERROR::SHADER::PROGRAM::CREATE_PROGRAM_FAILED");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::ProgramCreationError));
    cleanup();
    return;
#endif
  }

  // I'd love to use iterators and other newer C++ features to do the
  // below, but for now it works and is easy to understand.
  while (!shaders.empty()) {
    unique_ptr<Shader> shader = std::move(shaders.back());
    gl_context->glAttachShader(program, shader->shader);
    // Operations on the back are efficient for deques
    shaders.pop_back();
    shader_map.emplace(shader->shader, std::move(shader));
  }

  link();
}

Program::~Program() { cleanup(); }

void Program::cleanup() noexcept {
  if (program != 0) {
    // We also need to check for a valid gl_context, which may get cleared
    // in move constructors and assignments
    if (gl_context != nullptr) {
      gl_context->glDeleteProgram(program);
    }
    program = 0;
  }
  gl_context = nullptr;
}

// move constructor
Program::Program(Program &&prg) noexcept : name{prg.name} {
  gl_context = prg.gl_context;
  program = prg.program;
  shader_map = std::move(prg.shader_map);
#ifdef NO_EXCEPTIONS
  last_operation_failed = prg.last_operation_failed;
  last_error = prg.last_error;
#endif

  prg.gl_context = nullptr;
  prg.program = 0;
}

// move assignment operator
Program &Program::operator=(Program &&prg) noexcept {
  if (&prg != this) {
    gl_context = prg.gl_context;
    name = prg.name;
    program = prg.program;
    shader_map = std::move(prg.shader_map);
#ifdef NO_EXCEPTIONS
    last_operation_failed = prg.last_operation_failed;
    last_error = prg.last_error;
#endif

    prg.gl_context = nullptr;
    prg.program = 0;
  }

  return *this;
}

void Program::link() {
  // spdlog::info("Linking OpenGL program");

  // This check is needed because we use move constructors and
  // assignment operators
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw ProgramUnspecifiedStateError("Program is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    cleanup();
    return;
#endif
  }

  gl_context->glLinkProgram(program);

  int success;

  gl_context->glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    // GLenum error = gl_context->glGetError();

    GLint infoLen = 0;

    gl_context->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen >= 1) {
      char *infoLog = new char[infoLen];
      if (infoLog == NULL) {
#ifndef NO_EXCEPTIONS
        spdlog::error("ERROR::SHADER::MEMORY_ALLOC::{}", name);
#endif
      } else {
        gl_context->glGetProgramInfoLog(program, infoLen, NULL, infoLog);
#ifndef NO_EXCEPTIONS
        spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED::{}::{}", name,
                      infoLog);
#endif
        delete[] infoLog;
      }
    }

#ifndef NO_EXCEPTIONS
    throw ProgramLinkingError("ERROR::SHADER::PROGRAM::LINKING_FAILED");
#else
    set_error(std::optional<sdl_opengl_cpp::error>(error::ProgramLinkingError));
    cleanup();
    return;
#endif
  }
}

GLuint Program::use() {
  // This check is needed because we use move constructors and
  // assignment operators
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw ProgramUnspecifiedStateError("Program is in an unspecified state");
#else
    set_error(std::optional<error>(error::UnspecifiedStateError));
    cleanup();
    return 0;
#endif
  }

  gl_context->glUseProgram(program);

  return 0;
}

GLuint Program::use(GLuint program_name) {
  // spdlog::info("Using OpenGL program {}", out_of_use_id);

  // This check is needed because we use move constructors and
  // assignment operators
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw ProgramUnspecifiedStateError("Program is in an unspecified state");
#else
    set_error(std::optional<error>(error::UnspecifiedStateError));
    cleanup();
    return 0;
#endif
  }

  gl_context->glUseProgram(program_name);

  // The current implementation returns the OpenGL "name" (GLuint
  // program id) associated with this Program object.  A better API
  // might return the last "name" that was active.
  return program;
}

GLint Program::getUniformLocation(const std::string &uniform_name_to_get) {
  // This check is needed because we use move constructors and
  // assignment operators
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw ProgramUnspecifiedStateError("Program is in an unspecified state");
#else
    set_error(std::optional<error>(error::UnspecifiedStateError));
    cleanup();
    return -1;
#endif
  }

  GLint location =
      gl_context->glGetUniformLocation(program, uniform_name_to_get.c_str());
  if (location == -1) {
#ifndef NO_EXCEPTIONS
    spdlog::error("Couldn't get location {} of uniform");
    throw GetUniformLocationError("Couldn't get location of uniform");
#else
    set_error(std::optional<error>(error::GetUniformLocationError));
    cleanup();
    return -1;
#endif
  }

  return location;
}

// Implement checking for an unspecified state
bool Program::is_in_unspecified_state() const {
  if ((gl_context == nullptr) || (program == 0))
    return true;
  else
    return false;
}

// Experiments with scoped_use are disabled for now.

// scoped_use::scoped_use(std::unique_ptr<Program> &prg)
//   : program { prg },
//     out_of_use_id { 0 }
// {
//   program->use();
// }

// scoped_use::scoped_use(std::unique_ptr<Program> &prg, GLuint oouid)
//   : program { prg },
//     out_of_use_id { oouid }
// {
//   program->use();
// }

// scoped_use::~scoped_use()
// {
//   program->use(out_of_use_id);
//   // gl_context->glUseProgram(out_of_use_id);
// }
