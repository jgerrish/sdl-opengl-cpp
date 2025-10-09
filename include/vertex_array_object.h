#ifndef _SDL_OPENGL_CPP_VERTEX_ARRAY_OBJECT_H_
#define _SDL_OPENGL_CPP_VERTEX_ARRAY_OBJECT_H_

#include <memory>

#ifdef NO_EXCEPTIONS
#include <optional>
#endif

#include <stdexcept>
#include <vector>

#include "SDL_opengl.h"
#include <SDL.h>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#else
#include "move_checker.h"
#endif

#include "gl_context.h"

#include "vertex_buffer_object.h"

using namespace std;

namespace sdl_opengl_cpp {

// Maybe expand vertex_buffer_object namespace to include more
namespace vertex_array_object {

#ifndef NO_EXCEPTIONS

//! A GenVertexArraysError exception
//!
//! This exception is thrown when there is a problem with
//! constructing the buffer for the Vertex Array Object with
//! glGenBuffers.
//!
class GenVertexArraysError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
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
class InvalidOperationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A VertexArrayObjectUnspecifiedStateError exception
//!
//! This exception is thrown when the VertexArrayObject is in an
//! valid but unspecified state after a move operation.
//!
class VertexArrayObjectUnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};
#endif
} // namespace vertex_array_object

using namespace vertex_array_object;

//! A VertexArrayObject class owns and manages OpenGL Vertex Array
//! Objects.
//!
//! Vertex Array Objects allow you to bind uploaded Vertex Buffer
//! Objects to OpenGL vertices on the hardware.  These vertices can
//! be used in later stages in the OpenGL pipeline.
//!
//! This class uses a previously constructed VertexBufferObject
//! class with active data buffers.  The VertexBufferObject is bound
//! to a new OpenGL Vertex Array Object.
//!
//! The VertexArrayObject takes ownership of the VertexBufferObject.
//! Tke VertexBufferObject is in an "valid but unspecified state"
//! after constructing the VertexArrayObject with it.
//!
//! Owned OpenGL resources are automatically cleaned up on object
//! deletion.  The owned VertexBufferObject is also cleaned up.
//!
//! It is up to the user to cleanup their own data.
#ifndef NO_EXCEPTIONS
class VertexArrayObject : private MoveChecker {
#else
class VertexArrayObject : public Errors {
#endif
  // This is one way of allowing us to test private member variables
  // and state.
  friend class VertexArrayObjectTester;

public:
  VertexArrayObject(const string &name, const std::shared_ptr<GLContext> &ctx,
                    VertexBufferObject &&vbo);
  ~VertexArrayObject();

  //! Cleanup the vertex array object
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  // Explicitly delete the generated default copy constructor
  VertexArrayObject(const VertexArrayObject &) = delete;

  // Explicitly delete the generated default copy assignment operator
  VertexArrayObject &operator=(const VertexArrayObject &) = delete;

  // move constructor
  VertexArrayObject(VertexArrayObject &&) noexcept;

  // move assignment operator
  VertexArrayObject &operator=(VertexArrayObject &&) noexcept;

  bool is_in_unspecified_state() const override;

  void bind();

private:
  string name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  VertexBufferObject vbo;

  // OpenGL Vertex Array Object
  GLuint VAO = 0;
};

} // namespace sdl_opengl_cpp

#endif
