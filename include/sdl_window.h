#ifndef _SDL_OPENGL_CPP_SDL_WINDOW_H_
#define _SDL_OPENGL_CPP_SDL_WINDOW_H_

#include <functional>
#include <memory>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#include <optional>
#else
#include "move_checker.h"
#include "spdlog/spdlog.h"
#endif

#include <stdexcept>

#include "sdl_base.h"

using namespace std;

namespace sdl_opengl_cpp {

//! Forward declare the tester friend.  If we explicitly include the
//! namespace down below in the friend statement, compilation fails
//! with: "'SDLWindowTester in namespace 'sdl_opengl_cpp' does not name
//! a type"
//!
//! This fixes that
class SDLWindowTester;

// nested namespaces added in C++17
// Perhaps this should be an sdl namespace and the SDLWindow class
// should be renamed to Window
namespace sdl_window {

#ifndef NO_EXCEPTIONS

//! A SDLWindowCreationError exception
//!
//! An error occurred trying to create a window
//!
class SDLWindowCreationError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A CreateOpenGLContextError exception
//!
//! An error occurred trying to create an OpenGL Context
//!
class CreateOpenGLContextError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! A SDLWindowUnspecifiedStateError exception
//!
//! This exception is thrown when the SDLWindow is in an
//! valid but unspecified state after a move operation.
//!
class SDLWindowUnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

#ifndef NO_EXCEPTIONS
class SDLWindow : private MoveChecker {
#else
class SDLWindow : public Errors {
#endif
  // This is one way of allowing us to test private member variables
  // and state.
  friend class sdl_opengl_cpp::SDLWindowTester;

public:
  SDLWindow(const std::shared_ptr<SDL> &sdl_, const char *title, int x, int y,
            int w, int h, Uint32 flags);
  ~SDLWindow();

  //! Cleanup the window
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  // Explicitly delete the generated default copy constructor we
  // don't want multiple copies floating around since the window is
  // a managed SDL resource
  SDLWindow(const SDLWindow &) = delete;

  // Explicitly delete the generated default copy assignment
  // operator we don't want multiple copies floating around since
  // the widow is a managed SDL resource.
  SDLWindow &operator=(const SDLWindow &) = delete;

  // move constructor
  SDLWindow(SDLWindow &&) noexcept;

  // move assignment operator
  SDLWindow &operator=(SDLWindow &&) noexcept;

  //! Create an OpenGL context for the window, and make it current.
  //!
  //! \returns the OpenGL context associated with `window` or NULL on
  //!          error
  DECLSPEC SDL_GLContext SDLCALL GL_CreateContext();

  //! Set up an OpenGL context for rendering into window.
  //!
  //! \param context the OpenGL context to associate with the window.
  //!
  //! \returns 0 on success or a negative error code on failure
  DECLSPEC int SDLCALL GL_MakeCurrent(SDL_GLContext context);

  //! Get the size of the window's underlying drawable in pixels.
  //!
  //! \param w a pointer to a variable for storing the width in pixels
  //! \param h a pointer to a variable for storing the height in pixels
  DECLSPEC void SDLCALL GL_GetDrawableSize(int *w, int *h);

  //! Update the window with OpenGL rendering.
  DECLSPEC void SDLCALL GL_SwapWindow();

  //! True if the the object is in an unspecified state
  bool is_in_unspecified_state() override;

private:
  //! The underlying SDL object for this window
  std::shared_ptr<SDL> sdl = nullptr;

  //! The underlying SDL_Window handle
  SDL_Window *window;
};

} // namespace sdl_window

} // namespace sdl_opengl_cpp

#endif
