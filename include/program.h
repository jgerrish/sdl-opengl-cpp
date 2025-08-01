#ifndef _SDL_OPENGL_CPP_PROGRAM_H_
#define _SDL_OPENGL_CPP_PROGRAM_H_

#include <deque>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "SDL_opengl.h"
#include <SDL.h>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#else
#include "move_checker.h"
#endif

#include "gl_context.h"
#include "shader.h"

using namespace std;

namespace sdl_opengl_cpp {

const int MAX_SHADERS = 256;

#ifndef NO_EXCEPTIONS

class ProgramCreationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class ProgramLinkingError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class GetUniformLocationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class ProgramUnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

// class scoped_use;

#ifndef NO_EXCEPTIONS
class Program : private MoveChecker {
#else
class Program : public Errors {
#endif
  // This is one way of allowing us to test private member variables
  // and state.
  friend class ProgramTester;

public:
  //! Construct a program
  //!
  //! This creates a new OpenGL program object and links it.
  //!
  //! \param name The name of the program
  //! \param ctx A pointer to the OpenGL context to use for OpenGL operations
  //! \param shaders A deque of Shader unique_ptrs to attach to the program.
  //!
  //! \throws a ProgramCreationError if there was an error creating
  //!         the program.
  //! \throws a ProgramLinkingError if there was an error
  //!         linking the program.
  //!
  //! \return A new Program object
  Program(const string &name, const std::shared_ptr<GLContext> &ctx,
          deque<unique_ptr<Shader>> &shaders);
  ~Program();

  // Explicitly delete the generated default copy constructor
  Program(const Program &) = delete;

  // Explicitly delete the generated default copy assignment operator
  Program &operator=(const Program &) = delete;

  // move constructor
  Program(Program &&) noexcept;

  // move assignment operator
  Program &operator=(Program &&) noexcept;

  //! Cleanup the program
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  //! Link the OpenGL program
  //!
  //! This method links the OpenGL program.
  //!
  //! \throws a ProgramUnspecifiedStateError if the Program is in a
  //!         valid but unspecified state after a C++ move assignment or
  //!         construction.
  //! \throws a ProgramLinkingError if there was an error
  //!         linking the program.
  void link();

  //! This uses this Program in the current OpenGL active state.
  //!
  //! The current implementation returns the OpenGL "name" (GLunt
  //! program id) associated with this Program object.  A better API
  //! might return the last "name" that was active.
  //!
  //! \throws a UnspecifiedStateError if the Program is in a valid but
  //!         unspecified state after a C++ move assignment or
  //!         construction.
  GLuint use();

  //! Use the given OpenGL program "name" (GLuint program id) in the
  //! current graphics state.
  //!
  //! The current implementation returns the OpenGL "name" (GLuint
  //! program id) associated with this Program object.  A better API
  //! might return the last "name" that was active.
  //!
  //! \throws a UnspecifiedStateError if the Program is in a valid but
  //!         unspecified state after a C++ move assignment or
  //!         construction.
  GLuint use(GLuint program_name);

  GLint getUniformLocation(const std::string &uniform_name_to_get);

  bool is_in_unspecified_state() override;

private:
  //! The Program name
  string name;

  // The OpenGL context this program uses
  std::shared_ptr<GLContext> gl_context = nullptr;

  // The OpenGL program object this class owns
  GLuint program = 0;

  // This may not be the right way to do it. We might want to use a
  // reference-counting smart pointer here.  That would be more
  // modern and easily done.  For now, we use a unique_ptr.
  // That means an individual Shader can only be used with one
  // Program at a time.
  // This is a const unique_ptr.  It then can't transfer ownership
  // to another unique_ptr, this is the final destination.
  // We don't use program pipelines or more advanced OpenGL features
  // currently.
  unordered_map<GLuint, const unique_ptr<Shader>> shader_map{};

  // scoped_use in_use;
};

// // A little experiment with scoped use
// // This isn't efficient, but it's a test.
// //
// // TODO: WARNING: This requires more review on C++ standards, memory
// // models, and compiler behavior.  Since the scoped_use never
// // actually gets used, it may get optimized out by some compilers.
// // Or even if it does get used, it might get re-ordered without some
// // explicit guards or barriers like the lower-level pthread locking
// // libraries use.
// //
// //! scoped_use is a class similar to scoped_lock that uses a Program
// //! for the length of a scope.  After the scope is exited, the
// //! default program (0) or a custom program is switched to.
// class scoped_use {
// public:
//   // I haven't found a way of querying the OpenGL hardware for
//   // the currently in-use program and switches back to that after
//   // the program goes out of use.
//   //
//   scoped_use(std::unique_ptr<Program> &prg);
//   scoped_use(std::unique_ptr<Program> &prg, GLuint out_of_use_id);
//   ~scoped_use();
// private:
//   std::unique_ptr<Program> program;
//   GLuint out_of_use_id;
// };

} // namespace sdl_opengl_cpp

#endif
