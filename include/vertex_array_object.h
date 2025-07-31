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
#include "error.h"
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
class VertexArrayObject {
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

  void bind();

#ifdef NO_EXCEPTIONS
  //! Return true if the last VertexArrayObject method call was
  //! successful.
  //!
  //! This method will also return false if the VertexArrayObject
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
  //! \return true if the last VertexArrayObject function call was
  //!         successful AND the object is not in a "valid but
  //!         unspecified state".
  bool valid();

  //! If the object is not valid(), this method will get the last error
  //! that occurred.
  //!
  //! If the error caused the object to be put into a "valid but
  //! uncertain state" then this method will return the error that
  //! caused that, NOT UnspecifiedStateError.  If the operation that
  //! caused the "valid but unspecified state" was just a move
  //! assignment or move constructor, then it should return
  //! UnspecifiedStateError.
  //!
  //! This is a little confusing because getting put into a "valid
  //! but unspecified state" is not necessarily an error.  But for
  //! the valid() call and any subsequent call, it is an error, but
  //! the object was attempted to be used in a "valid but
  //! unspecified state".
  //!
  //! \return the last error that occurred during a method call.
  std::optional<error> get_last_error();
#endif

private:
  string name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  VertexBufferObject vbo;

  // OpenGL Vertex Array Object
  GLuint VAO = 0;

#ifdef NO_EXCEPTIONS
  //! Whether the last operation failed
  bool last_operation_failed = false;

  //! The last error that occured, or std::nullopt if there was none.
  std::optional<error> last_error = std::nullopt;
#endif
};

} // namespace sdl_opengl_cpp

#endif
