#ifndef _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_H_
#define _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_H_

#include <memory>

#ifdef NO_EXCEPTIONS
#include <optional>
#endif

#include <stdexcept>
#include <vector>

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

#include "gl_context.h"

using namespace std;

namespace sdl_opengl_cpp {

// nested namespaces added in C++17
// Maybe expand vertex_buffer_object namespace to include more
namespace vertex_buffer_object {

#ifndef NO_EXCEPTIONS

//! A BufferDataError exception
//!
//! This exception is thrown when there is a problem with the data
//! that was attempted to be loaded into a Vertex Buffer Object.
//!
class BufferDataError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A GenBuffersError exception
//!
//! This exception is thrown when there is a problem with
//! constructing the buffer with glGenBuffers.
//!
// We want the base class public because we translate the what() message
// for doctest.  We could also use a friend, but that requires
// including doctest in the non-testing components of this library.
// Or some translator translator...
//
// This exception, unlike all the others, inherits from
// runtime_error as protected, and has a what() method to show what
// is needed for doctest to test exceptions.
class GenBuffersError : protected runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;

public:
  const char *what() const noexcept { return runtime_error::what(); }
};

//! A InvalidOperation exception
//!
//! TODO: This should probably be a library-specific exception.  We
//!       know based on what function was previously called what the
//!       "subtype" of the GL_INVLIAD_OPERATION error is likely to
//!       be.  E.g. an invalid or deleted VAO handle.
//!
//! This exception is thrown when there is a problem with the data
//! that was attempted to be loaded into a Vertex Buffer Object.
//!
class InvalidOperationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A VertexBufferObjectUnspecifiedStateError exception
//!
//! This exception is thrown when the VertexBufferObject is in an
//! valid but unspecified state after a move operation.
//!
class VertexBufferObjectUnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#else

// I should probably implement io streams for errors.  And
// duplicating all the error registration and error setting code
// in every class is very not DRY.  So we should add some error
// manager class and clean this up.
enum class error {
  BufferDataError,
  GenBuffersError,
  InvalidOperationError,
  VertexBufferObjectUnspecifiedStateError
};

// See the comment in vertex_buffer_object.cpp about spdlog formatters
// not working.
// struct my_type {
//   error err;
//   explicit my_type(error e) : err(e) { }
// };

extern ostream &operator<<(std::ostream &os, const error &e);
extern std::string error_as_string(const error &e);

#endif

} // namespace vertex_buffer_object

using namespace vertex_buffer_object;

//! A VertexBufferObject class owns and manages OpenGL Vertex Buffer
//! Objects.
//!
//! Vertex Buffer Objects allow you to tie and upload local program
//! data to OpenGL hardware.
//!
//! This class lets you use C++ Standard Template Library data
//! structures like vector to store OpenGL floats (GLfloat) and
//! upload them to the OpenGL hardware easily.
//!
//! Owned OpenGL resources are automatically cleaned up on object
//! deletion.  It is up to the user to cleanup their own data.
class VertexBufferObject /* : public OpenGLObject */ {
  // This is one way of allowing us to test private member variables
  // and state.
  friend class VertexBufferObjectTester;

  // To test that the VBO was moved when constructing a VAO
  friend class VertexArrayObjectTester;

public:
  //! Construct a vertex buffer object
  //!
  //! This creates a new OpenGL vertex buffer object and fills it with data
  //!
  //! \param name The name of the vertex buffer object
  //! \param ctx A pointer to the OpenGL context to use for OpenGL operations
  //! \param data A vector of GLfloats containing the data to load
  //!             into the vertex buffer object
  //!
  //! \throws a BufferDataError if the data is too large to store in
  //!         the buffer.
  //!
  //! \throws a GenBuffersError if there was an error generating the
  //!         buffer.
  //!
  //! \return A new VertexBufferObject object which owns the data
  VertexBufferObject(const string &name, const std::shared_ptr<GLContext> &ctx,
                     const vector<GLfloat> &data);
  ~VertexBufferObject();

  //! Cleanup the vertex buffer object
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

#ifdef NO_EXCEPTIONS
  //! Return true if the last VertexBufferObject method call was
  //! successful.
  //!
  //! This method will also return false if the VertexBufferObject
  //! is in a "valid but unspecified state", for example after a
  //! move assignment or move construction.  This is so that the
  //! user doesn't have to call it twice.  It should ALWAYS return
  //! false if the object can't be used or the last operation had an
  //! error.
  //!
  //! If this library has been compiled with NO_EXCEPTIONS defined,
  //! this must be called after every function that can throw an
  //! error.
  //!
  //! \return true if the last VertexBufferObject function call was
  //!         successful AND the object is not in a "valid but
  //!         unspecified state".
  bool valid();

  //! If the object is not valid(), this method will get the last error
  //! that occurred.
  //!
  //! If the error caused the object to be put into a "valid but
  //! uncertain state" then this method will return the error that
  //! caused that, NOT VertexBufferObjectUnspecifiedStateError.  If
  //! the operation that caused the "valid but unspecified state"
  //! was just a move assignment or move constructor, then it should
  //! return VertexBufferObjectUnspecifiedStateError.
  //!
  //! This is a little confusing because getting put into a "valid
  //! but unspecified state" is not necessarily an error.  But for
  //! the valid() call and any subsequent call, it is an error, but
  //! the object was attempted to be used in a "valid but
  //! unspecified state".
  //!
  //! \return the last error that occurred during a method call.
  std::optional<vertex_buffer_object::error> get_last_error();

  //! Set the error code
  //!
  //! This should be the only function that changes error_code.
  //! It also calls any error handler functions.
  //!
  //!
  //! \return true if the error state changed
  //!         false if there was no change in the error state
  bool set_error(const std::optional<vertex_buffer_object::error> &error);

  //! Register an error handler
  //!
  //! This error handler will get called everytime the state of
  //! last_error changes.
  //!
  //! We don't call the error handler on every set_error call
  //! Only if all the following conditions are met:
  //! 1. The error state changed
  //! 2. An error handler is registered
  //! 3. The error is an actual error, and not std::nullopt
  //!
  //! This means it doesn't get called on things like resetting the
  //! error, setting an error that was already set, or when there is no
  //! error handler.
  //!
  //! There is no information about what function caused the error.
  //!
  //! \return 0 on success, nonzero on an error registering the
  //!         error handler
  int register_error_handler(
      const std::function<void(const vertex_buffer_object::error &error)>
          &handler);

#endif

  // Explicitly delete the generated default copy constructor we
  // don't want multiple copies floating around since the buffer is
  // a managed OpenGL resource
  VertexBufferObject(const VertexBufferObject &) = delete;

  // Explicitly delete the generated default copy assignment
  // operator we don't want multiple copies floating around since
  // the buffer is a managed OpenGL resource.
  VertexBufferObject &operator=(const VertexBufferObject &) = delete;

  // move constructor
  VertexBufferObject(VertexBufferObject &&) noexcept;

  // move assignment operator
  VertexBufferObject &operator=(VertexBufferObject &&) noexcept;

  void bind();

private:
  string name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  // OpenGL Vertex Buffer Object
  GLuint VBO = 0;

  // mutex vbo_mutex {};
  // scoped_lock lck { vbo_mutex };

#ifdef NO_EXCEPTIONS
  //! Whether the last operation failed
  bool last_operation_failed = false;

  //! The last error that occured, or std::nullopt if there was none.
  std::optional<vertex_buffer_object::error> last_error = std::nullopt;

  //! The error callback function
  std::optional<std::function<void(const vertex_buffer_object::error &error)>>
      error_handler = std::nullopt;

#endif
};

} // namespace sdl_opengl_cpp
#endif
