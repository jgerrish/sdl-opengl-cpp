#include "spdlog/spdlog.h"

#include "shader.h"

using namespace std;
using namespace sdl_opengl_cpp;

Shader::Shader(const string &name, const std::shared_ptr<GLContext> &ctx,
               const string &src, const GLenum type)
    : shader_name{name}, gl_context{ctx}, shader_type{type} {
  shader = gl_context->glCreateShader(type);
  if (shader == 0) {
#ifndef NO_EXCEPTIONS
    spdlog::error("ERROR::SHADER::CREATE_SHADER_FAILED::{}", name);
    throw ShaderCreationError("ERROR::SHADER::CREATE_SHADER_FAILED");
#else
    set_error(std::optional<error>(error::ShaderCreationError));
    cleanup();
    return;
#endif
  }

  // If we get an error in compilation called from the constructor, cleanup the
  // object We're still trying to do a RAII approach.
#ifndef NO_EXCEPTIONS
  try {
    this->compile(src);
  } catch (ShaderCompilationError &e) {
    cleanup();
    throw(e);
  } catch (ShaderUnspecifiedStateError &e) {
    cleanup();
    throw(e);
  }
#else
  this->compile(src);
#endif
}

Shader::~Shader() { cleanup(); }

void Shader::cleanup() noexcept {
  if (shader != 0) {
    // We also need to check for a valid gl_context, which may get cleared
    // in move constructors and assignments
    if (gl_context != nullptr)
      gl_context->glDeleteShader(shader);
    shader = 0;
  }
  gl_context = nullptr;
  shader_type = 0;
}

// move constructor
Shader::Shader(Shader &&s) noexcept
    : shader_name{s.shader_name}, shader_type{s.shader_type} {
  gl_context = s.gl_context;
  shader = s.shader;
#ifdef NO_EXCEPTIONS
  last_operation_failed = s.last_operation_failed;
  last_error = s.last_error;
#endif

  s.gl_context = nullptr;
  s.shader = 0;
  s.shader_type = 0;
}

// move assignment operator
Shader &Shader::operator=(Shader &&s) noexcept {
  if (&s != this) {
    gl_context = s.gl_context;
    shader = s.shader;
    shader_name = s.shader_name;
    shader_type = s.shader_type;
#ifdef NO_EXCEPTIONS
    last_operation_failed = s.last_operation_failed;
    last_error = s.last_error;
#endif

    s.gl_context = nullptr;
    s.shader = 0;
    s.shader_type = 0;
  }

  return *this;
}

void Shader::compile(const string &src) {
  // spdlog::info("Compiling OpenGL Shader");

  // This check is needed because we use move constructors and
  // assignment operators
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw ShaderUnspecifiedStateError("Shader is in an unspecified state");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::UnspecifiedStateError));
    cleanup();
    return;
#endif
  }

  // spdlog::info("Compiling OpenGL Shader: {}", src);

  // std::string can invalidate the pointer if (from cppreference.com):
  // 1.  A non-const reference to the string is passed to any standard library
  // function, or
  // 2.  Calling non-const memer functions on the string, including non-const
  // iterators
  //     and getters.
  // So be careful if you make the source a member variable that you
  // lock it and unlock for the next few lines.
  // Then make sure other uses of it are also protected by the same
  // mutex.

  // Enable if you move src to a member variable
  // {
  // std::scoped_lock lock(shader_src_mutex);
  const char *c_str_src = src.c_str();

  gl_context->glShaderSource(shader, 1, &c_str_src, NULL);
  // }

  gl_context->glCompileShader(shader);

  int success;

  gl_context->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLint infoLen = 0;

    gl_context->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen >= 1) {
#ifndef NO_EXCEPTIONS
      spdlog::info("Allocating infoLog of size {}", infoLen);
#endif
      char *infoLog = new char[infoLen];
      if (infoLog == NULL) {
#ifndef NO_EXCEPTIONS
        spdlog::error("ERROR::SHADER::MEMORY_ALLOC::{}", shader_name);
#endif
      } else {
        gl_context->glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
#ifndef NO_EXCEPTIONS
        spdlog::error("ERROR::SHADER::COMPILATION_FAILED::{}::{}::src: {}",
                      shader_name, infoLog, src);
#endif

        delete[] infoLog;
      }
    }
#ifndef NO_EXCEPTIONS
    throw ShaderCompilationError("ERROR::SHADER::COMPILATION_FAILED");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::ShaderCompilationError));
    cleanup();
    return;
#endif
    // } else {
    // spdlog::info("INFO::SHADER::COMPILATION_SUCCESSFUL::{}", shader_name);
  }
}

GLuint Shader::openGLName() { return shader; }

// Implement checking for an unspecified state
bool Shader::is_in_unspecified_state() const {
  if ((gl_context == nullptr) || (shader == 0))
    return true;
  else
    return false;
}
