#include <cmath>

#include "spdlog/spdlog.h"

#include "vertex_array_object.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::vertex_array_object;

VertexArrayObject::VertexArrayObject(const string &array_name,
                                     const std::shared_ptr<GLContext> &ctx,
                                     // TODO: Come up with a style
                                     // guide to deal with shadowing
                                     // of parameters and member
                                     // variables.  This is kind of
                                     // the opposite of Google, which
                                     // uses underscore for member
                                     // variables.
                                     VertexBufferObject &&vbo_)
    : name{array_name}, gl_context{ctx}, vbo{std::move(vbo_)} {
  // Previously we were using glGenBuffers to generate vertex arrays.
  // It seemed to actually work with results visible on screen.  But
  // it's not the proper way to generate vertex arrays.  Use
  // glGenVertexArrays.
  gl_context->glGenVertexArrays(1, &VAO);

  GLenum error = gl_context->glGetError();

  // If an OpenGL OUT_OF_MEMORY error is generated, the state of any
  // pointer argument value is unchanged.  This is according to the GL
  // 4.1 core profile specification, page 19.
  if ((error == GL_OUT_OF_MEMORY) || (VAO == 0)) {
#ifndef NO_EXCEPTIONS
    throw GenVertexArraysError("ERROR::VERTEX_ARRAY::GEN_VERTEX_ARRAYS_FAILED");
#else
    last_operation_failed = true;
    last_error.emplace(vertex_array_object::error::GenVertexArraysError);
    cleanup();
    return;
#endif
  }

  gl_context->glBindVertexArray(VAO);
  // glBindVertexArray can return an error
  error = gl_context->glGetError();

  if (error == GL_INVALID_OPERATION) {
#ifndef NO_EXCEPTIONS
    throw vertex_array_object::InvalidOperationError(
        "ERROR::VERTEX_ARRAY::INVALID_OPERATION_ERROR");
#else
    last_operation_failed = true;
    last_error.emplace(vertex_array_object::error::InvalidOperationError);
    cleanup();
    return;
#endif
  }

  gl_context->glEnableVertexAttribArray(0);

  this->vbo.bind();

  gl_context->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  error = gl_context->glGetError();

  if (error == GL_INVALID_OPERATION) {
#ifndef NO_EXCEPTIONS
    throw vertex_array_object::InvalidOperationError(
        "ERROR::VERTEX_ARRAY::INVALID_OPERATION_ERROR");
#else
    last_operation_failed = true;
    last_error.emplace(vertex_array_object::error::InvalidOperationError);
    cleanup();
    return;
#endif
  }

  gl_context->glBindBuffer(GL_ARRAY_BUFFER, 0);

  gl_context->glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject() { cleanup(); }

void VertexArrayObject::cleanup() noexcept {
  if (VAO != 0) {
    if (gl_context != nullptr) {
      gl_context->glDeleteVertexArrays(1, &VAO);
    }

    VAO = 0;
  }

  gl_context = nullptr;
}

// move constructor
VertexArrayObject::VertexArrayObject(VertexArrayObject &&vao) noexcept
    : name{vao.name}, vbo{std::move(vao.vbo)} {
  gl_context = vao.gl_context;
  VAO = vao.VAO;
#ifdef NO_EXCEPTIONS
  last_operation_failed = vao.last_operation_failed;
  last_error = vao.last_error;
#endif

  vao.gl_context = nullptr;
  vao.VAO = 0;
}

// move assignment operator
VertexArrayObject &
VertexArrayObject::operator=(VertexArrayObject &&vao) noexcept {
  if (&vao != this) {
    gl_context = vao.gl_context;
    name = vao.name;
    VAO = vao.VAO;
    vbo = std::move(vao.vbo);
#ifdef NO_EXCEPTIONS
    last_operation_failed = vao.last_operation_failed;
    last_error = vao.last_error;
#endif

    vao.gl_context = nullptr;
    vao.VAO = 0;
  }

  return *this;
}

void VertexArrayObject::bind() {
  // It looks like scoped_lock can throw an exception.  From the
  // std::lock cppreference page:
  // "The objects are locked by an unspecified series of calls to
  // lock, try_lock, and unlock. If a call to lock or unlock results
  // in an exception, unlock is called for any locked objects before
  // rethrowing."
  //
  // scoped_lock lck { vbo_mutex };

  if ((gl_context == nullptr) || (VAO == 0)) {
#ifndef NO_EXCEPTIONS
    throw VertexArrayObjectUnspecifiedStateError(
        "Vertex Array Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(
        vertex_array_object::error::VertexArrayObjectUnspecifiedStateError);
    return;
#endif
  }

  gl_context->glBindVertexArray(VAO);
  // gl_context->glBindBuffer(GL_ARRAY_BUFFER, VAO);

#ifdef NO_EXCEPTIONS
  last_operation_failed = false;
#endif
}

#ifdef NO_EXCEPTIONS

bool VertexArrayObject::valid() {
  if ((gl_context == nullptr) || (VAO == 0)) {
    if (!last_error) {
      // If we didn't do this it would require two calls to valid()
      last_operation_failed = true;
      last_error.emplace(
          vertex_array_object::error::VertexArrayObjectUnspecifiedStateError);
    }
  }

  return !last_operation_failed;
}

std::optional<vertex_array_object::error> VertexArrayObject::get_last_error() {
  if ((gl_context == nullptr) || (VAO == 0)) {
    if (!last_error) {
      // last_error hasn't been set yet.  There wasn't an error that
      // caused gl_context or VBO to be reset, so we assume it was a
      // move construction or assignment.
      //
      // If we didn't do this it would require two calls to get_last_error()
      last_operation_failed = true;
      last_error.emplace(
          vertex_array_object::error::VertexArrayObjectUnspecifiedStateError);
    }
  }

  return last_error;
}

#endif
