#ifndef _SDL_OPENGL_CPP_SHADER_H_
#define _SDL_OPENGL_CPP_SHADER_H_

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "SDL_opengl.h"
#include <SDL.h>

#ifdef NO_EXCEPTIONS
#include "error.h"
#endif

#include "opengl.h"

#include "gl_context.h"

using namespace std;

namespace sdl_opengl_cpp {

#ifndef NO_EXCEPTIONS

class ShaderUnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class ShaderCreationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class ShaderCompilationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

class Shader {
  // This is one way of allowing us to test private member variables
  // and state.
  friend class ShaderTester;

public:
  //! Construct a shader
  //!
  //! This creates a new OpenGL shader object and compiles the code
  //! for it.
  //!
  //! \param name The name of the shader
  //! \param ctx A pointer to the OpenGL context to use for OpenGL operations
  //! \param src The shader source to use
  //! \param type The shader type
  //!
  //! \throws a ShaderCreationError if there was an error creating
  //!         the shader.
  //! \throws a ShaderCompilationError if there was an error
  //!         compiling the shader source.
  //!
  //! \return A new Shader object
  Shader(const string &name, const std::shared_ptr<GLContext> &ctx,
         const string &src, const GLenum type);

  ~Shader();

  // See CppCoreGuidelines C.21: If you define or `=delete` any
  // copy, move, or destructor function, define or `=delete` them
  // all.
  //
  // Note from same section: : Relying on an implicitly generated
  // copy operation in a class with a destructor is deprecated.

  // Explicitly delete the generated default copy constructor
  Shader(const Shader &) = delete;

  // Explicitly delete the generated default copy assignment operator
  Shader &operator=(const Shader &) = delete;

  //! move constructor
  //!
  //! After this move constructor is called, the Shader that is
  //! passed in is in a "valid but unspecified" state.
  //!
  //! This shader object itself is in a valid and good state,
  //! specifically the exact state of the other shader (maybe with
  //! additional book-keeping or auditing in the future).
  //!
  //! It is safe to destroy the object and have the destructor run.
  //! It is also safe to assign another object to the other Shader.
  //! But every other function that depends on the underlying OpenGL
  //! shader object MUST fail with an exception on a "valid but
  //! unspecified" Shader that has been moved.
  Shader(Shader &&) noexcept;

  //! move assignment operator
  //!
  //! After this move assignment operator is called, the Shader that
  //! is passed in to move to this Shader is in an "valid but
  //! unspecified" state.
  //!
  //! This shader object itself is in a valid and good state,
  //! specifically the exact state of the other shader (maybe with
  //! additional book-keeping or auditing in the future).
  //!
  //! It is safe to destroy the object and have the destructor run.
  //! It is also safe to assign another object to this object.
  //!
  //! But every other function that depends on the underlying OpenGL
  //! shader object MUST fail with an exception on a "valid but
  //! unspecified" Shader that has been moved.
  //! For an example of how to deal with a Shader in this state,
  //! look at the compile(const char *src) member function.
  Shader &operator=(Shader &&) noexcept;

  //! Cleanup the shader
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  //! Compile the shader with new shader source
  //!
  //! Unlike the OpenGL API, there are not separate steps to upload
  //! source and then compile the source.
  //! The shader gets compiled with the passed in shader source and
  //! the shader type remains the same.
  //!
  //! \param src The shader source to use
  //!
  //! \throws a ShaderCompilationError if there was an error
  //!         compiling the shader source.
  //! \throws a UnspecifiedStateError if the shader is in an
  //!         unspecified state.
  void compile(const string &src);

  GLuint openGLName();

  // The OpenGL shader this class owns
  GLuint shader = 0;

#ifdef NO_EXCEPTIONS
  //! Return true if the last Shader method call was
  //! successful.
  //!
  //! This method will also return false if the Shader
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
  //! \return true if the last Shader function call was
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
  string shader_name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  // The OpenGL shader type
  // This cannot change through normal public APIs, unlike the
  // OpenGL compileShader API.
  // It must be set on object creation and assignment.
  GLenum shader_type = 0;

  // Use this to lock the src if you make it a member variable
  // std::mutex shader_src_mutex;

#ifdef NO_EXCEPTIONS
  //! Whether the last operation failed
  bool last_operation_failed = false;

  //! The last error that occured, or std::nullopt if there was none.
  std::optional<error> last_error = std::nullopt;
#endif
};

} // namespace sdl_opengl_cpp

#endif
