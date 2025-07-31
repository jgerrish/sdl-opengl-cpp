#include <cmath>

#include "spdlog/fmt/ostr.h" // support for user defined types
#include "spdlog/spdlog.h"

#include "vertex_buffer_object.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::vertex_buffer_object;

VertexBufferObject::VertexBufferObject(const string &buffer_name,
                                       const std::shared_ptr<GLContext> &ctx,
                                       const vector<GLfloat> &data)
    : name{buffer_name}, gl_context{ctx} {
  if (data.size() > static_cast<std::vector<GLfloat>::size_type>(
                        std::exp2(sizeof(GLsizei)))) {
#ifndef NO_EXCEPTIONS
    throw BufferDataError(
        "ERROR::VERTEX_BUFFER::BUFFER_DATA_ERROR::DATA_TOO_LARGE");
#else
    set_error(std::optional<error>(error::BufferDataError));
    cleanup();

    return;
#endif
  }

  // It looks like scoped_lock can throw an exception.  From the
  // std::lock cppreference page:
  // "The objects are locked by an unspecified series of calls to
  // lock, try_lock, and unlock. If a call to lock or unlock results
  // in an exception, unlock is called for any locked objects before
  // rethrowing."
  //
  // scoped_lock lck { vbo_mutex };

  // Right now, each VertexBufferObject only allocates one buffer
  //
  // We might want to add a collection type that lets us call
  // glGenBuffers with numbers besides one.
  //
  // There might be advantages with OpenGL hardware to sequential buffers
  ctx->glGenBuffers(1, &VBO);

  GLenum error = gl_context->glGetError();

  // If an OpenGL OUT_OF_MEMORY error is generated, the state of any
  // pointer argument value is unchanged.  This is according to the GL
  // 4.1 core profile specification, page 19.
  //
  // In addition, "there is no buffer object corresponding to the name zero"
  // page 42 of the 4.1 core profile specification
  if ((error == GL_OUT_OF_MEMORY) || (VBO == 0)) {
#ifndef NO_EXCEPTIONS
    throw GenBuffersError("ERROR::VERTEX_BUFFER::GEN_BUFFERS_FAILED");
#else
    set_error(std::optional<sdl_opengl_cpp::error>(error::GenBuffersError));
    cleanup();
    // If we return here, and last_operation_failed is set to false in
    // the class declaration, it is set to false after we set it to
    // true above.  Using std::optional doesn't fix this.
    return;
#endif
  }

  GLsizei buffer_size =
      static_cast<GLsizei>(sizeof(GLfloat) * (data.size() * 3));

  // spdlog::info("glBindBuffer in VertexBufferObject constructor: {}", VBO);
  ctx->glBindBuffer(GL_ARRAY_BUFFER, VBO);

  error = gl_context->glGetError();

  // GL_INVALID_OPERATION is set if buffer is not zero or a name
  // returned from a previous call to GenBuffers, or if such a name
  // has since been deleted with glDeleteBuffers.
  if (error == GL_INVALID_OPERATION) {
#ifndef NO_EXCEPTIONS
    throw InvalidOperationError(
        "ERROR::VERTEX_BUFFER::INVALID_OPERATION_ERROR");
#else
    set_error(
        std::optional<sdl_opengl_cpp::error>(error::InvalidOperationError));
    cleanup();
    // If we return here, and last_operation_failed is set to false in
    // the class declaration, it is set to false after we set it to
    // true above.  Using std::optional doesn't fix this.
    return;
#endif
  }

  ctx->glBufferData(GL_ARRAY_BUFFER, buffer_size, data.data(), GL_STATIC_DRAW);

  // When the VBO no longer needs to be an active target for reading
  // or writing, unbind it with the below
  // spdlog::info("glBindBuffer in VertexBufferObject constructor: 0", VBO);
  ctx->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::~VertexBufferObject() { cleanup(); }

void VertexBufferObject::cleanup() noexcept {
  if (VBO != 0) {
    // glInvalidateBufferData maybe isn't needed if we are explicitly
    // calling glDeleteBuffers
    // ctx->glInvalidateBufferData(&VBO);

    if (gl_context != nullptr) {
      gl_context->glDeleteBuffers(1, &VBO);
    }

    VBO = 0;
  }

  gl_context = nullptr;
}

#ifdef NO_EXCEPTIONS

bool VertexBufferObject::valid() {
  // Here, ! is operating on the std::optional to test if it has been
  // set or is std::nullopt
  // if (!last_operation_failed) {
  //   return true;
  // }

  if ((gl_context == nullptr) || (VBO == 0)) {
    if (!last_error) {
      // If we didn't do this it would require two calls to valid()
      set_error(std::optional<error>(error::UnspecifiedStateError));
    }
  }

  return !last_operation_failed;
}

std::optional<error> VertexBufferObject::get_last_error() {
  if ((gl_context == nullptr) || (VBO == 0)) {
    if (!last_error) {
      // last_error hasn't been set yet.  There wasn't an error that
      // caused gl_context or VBO to be reset, so we assume it was a
      // move construction or assignment.
      //
      // If we didn't do this it would require two calls to get_last_error()
      set_error(std::optional<error>(error::UnspecifiedStateError));
    }
  }

  return last_error;
}

bool VertexBufferObject::set_error(const std::optional<error> &error) {
  bool state_changed = false;

  if (last_error != error) {
    // One optional is set and the other isn't
    state_changed = true;
  } else {
    if (!last_error)
      // There was no last error, and the new optional is equal, so it
      // also is no error.
      state_changed = false;
    else
      // There was a last error, and the new error optional is also an error.
      // The state changed if they are different
      state_changed = (*last_error == *error);
  }

  if (!last_error) {
    last_operation_failed = true;
    last_error = error;
  }

  // We don't call the error handler on every set_error call
  // Only if all the following conditions are met:
  // 1. The error state changed
  // 2. An error handler is registered
  // 3. The error is an actual error, and not std::nullopt
  //
  // This means it doesn't get called on things like resetting the
  // error, setting an error that was already set, or when there is no
  // error handler.
  if (state_changed && error_handler && error) {
    (*error_handler)(*error);
  }

  return state_changed;
}

int VertexBufferObject::register_error_handler(
    const std::function<void(const error &error)> &handler) {
  error_handler.emplace(handler);
  return 0;
}

#endif

// move constructor
VertexBufferObject::VertexBufferObject(VertexBufferObject &&vbo) noexcept
    : name{vbo.name} {
  gl_context = vbo.gl_context;
  VBO = vbo.VBO;
#ifdef NO_EXCEPTIONS
  last_operation_failed = vbo.last_operation_failed;
  last_error = vbo.last_error;
#endif

  vbo.gl_context = nullptr;
  vbo.VBO = 0;
}

// move assignment operator
VertexBufferObject &
VertexBufferObject::operator=(VertexBufferObject &&vbo) noexcept {
  if (&vbo != this) {
    gl_context = vbo.gl_context;
    name = vbo.name;
    VBO = vbo.VBO;
#ifdef NO_EXCEPTIONS
    last_operation_failed = vbo.last_operation_failed;
    last_error = vbo.last_error;
#endif

    vbo.gl_context = nullptr;
    vbo.VBO = 0;

#ifdef NO_EXCEPTIONS
    // This is problematic.
    //
    // We rely on last_operation_failed and last_error being valid
    // even after a move assignment or construction.
    //
    // We might want an ErrorManager separate from any of the other
    // classes.
    //
    // last_operation_failed = false;
    // last_error.reset();
#endif
  }

  return *this;
}

void VertexBufferObject::bind() {
  if ((gl_context == nullptr) || (VBO == 0)) {
#ifndef NO_EXCEPTIONS
    throw VertexBufferObjectUnspecifiedStateError(
        "Vertex Buffer Object is in an unspecified state");
#else
    set_error(std::optional<error>(error::UnspecifiedStateError));
    return;
#endif
  }

  gl_context->glBindBuffer(GL_ARRAY_BUFFER, VBO);

#ifdef NO_EXCEPTIONS
  last_operation_failed = false;
#endif
}
