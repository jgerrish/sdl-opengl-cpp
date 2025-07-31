#ifndef _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_H_
#define _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_H_

#include <memory>

#include <stdexcept>
#include <vector>

#include "SDL_opengl.h"
#include <SDL.h>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#else
#include "move_checker.h"
#endif

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

// See the comment in vertex_buffer_object.cpp about spdlog formatters
// not working.
// struct my_type {
//   error err;
//   explicit my_type(error e) : err(e) { }
// };

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
#ifndef NO_EXCEPTIONS
class VertexBufferObject : private MoveChecker /* : public OpenGLObject */ {
#else
class VertexBufferObject : public Errors {
#endif
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

  bool is_in_unspecified_state() override;

  void bind();

private:
  string name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  // OpenGL Vertex Buffer Object
  GLuint VBO = 0;

  // mutex vbo_mutex {};
  // scoped_lock lck { vbo_mutex };
};

} // namespace sdl_opengl_cpp
#endif
