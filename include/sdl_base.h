#ifndef _SDL_OPENGL_CPP_SDL_H_
#define _SDL_OPENGL_CPP_SDL_H_

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

#include "SDL_opengl.h"
#include <SDL.h>

#include "sdl_wrapper.h"

#include "opengl.h"

using namespace std;

namespace sdl_opengl_cpp {

namespace sdl {

#ifndef NO_EXCEPTIONS

//! An SDLInitFailedError exception
//!
//! This exception is thrown when SDL initialization fails
class SDLInitFailedError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

//! An UnspecifiedStateError exception
//!
//! This exception is thrown when the SDL Object is in an valid but
//! unspecified state after a move operation.
class UnspecifiedStateError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

} // namespace sdl

using namespace sdl;

// Forward declarations for friend classes for testing
class SDLOpenGLTester;
class SDLTester;

#ifndef NO_EXCEPTIONS
class SDL : private MoveChecker {
#else
class SDL : public Errors {
#endif
  friend SDLOpenGLTester;
  friend SDLTester;

public:
  //! Construct an SDL object
  //!
  //! This creates a new SDL object.
  //! No subsystems are initialized.
  //!
  //! \throws a SDLInitFailedError if there was an error creating the
  //! SDL object.
  //!
  //! \return A new SDL object
  SDL();

  //! Construct an SDL object
  //!
  //! This creates a new SDL object with SDL parameters
  //!
  //! \param flags the flags to create the SDL object with the subsystems
  //! specified initialized.  Flags can be OR'd together.  This documentation is
  //! copied from SDL.
  //!        The following SDL flags are accepted:
  //!        - `SDL_INIT_TIMER`: timer subsystem
  //!        - `SDL_INIT_AUDIO`: audio subsystem
  //!        - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the
  //!        events
  //!          subsystem
  //!        - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically
  //!        initializes the
  //!          events subsystem
  //!        - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
  //!        - `SDL_INIT_GAMECONTROLLER`: controller subsystem; automatically
  //!          initializes the joystick subsystem
  //!        - `SDL_INIT_EVENTS`: events subsystem
  //!        - `SDL_INIT_EVERYTHING`: all of the above subsystems
  //!        - `SDL_INIT_NOPARACHUTE`: compatibility; this flag is ignored
  //!
  //!
  //! \throws a SDLInitFailedError if there was an error creating the
  //! SDL object.
  //!
  //! \return A new SDL object
  SDL(Uint32 flags);

  //! Create a new SDL object with a custom SDLWrapper
  //!
  //! This provides the same functionality as SDL() but can use custom
  //! SDLWrappe classes like mocks
  //!
  //! \param &sdl_wrapper The SDLWrapper object to use for SDL calls
  //!
  //! \throws a SDLInitFailedError if there was an error creating the
  //! SDL object.
  //!
  //! \return A new SDL object
  SDL(const std::shared_ptr<SDLWrapper> &sdl_wrapper_);

  //! Create a new SDL object with a custom SDLWrapper and flags
  //!
  //! This provides the same functionality as SDL(Uint32 flags) but can use
  //! custom SDLWrappe classes like mocks
  //!
  //! \param &sdl_wrapper The SDLWrapper object to use for SDL calls
  //! \param flags the flags to create the SDL object with the subsystems
  //! specified initialized.  Flags can be OR'd together.  This documentation is
  //! copied from SDL.
  //!        The following SDL flags are accepted:
  //!        - `SDL_INIT_TIMER`: timer subsystem
  //!        - `SDL_INIT_AUDIO`: audio subsystem
  //!        - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the
  //!        events
  //!          subsystem
  //!        - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically
  //!        initializes the
  //!          events subsystem
  //!        - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
  //!        - `SDL_INIT_GAMECONTROLLER`: controller subsystem; automatically
  //!          initializes the joystick subsystem
  //!        - `SDL_INIT_EVENTS`: events subsystem
  //!        - `SDL_INIT_EVERYTHING`: all of the above subsystems
  //!        - `SDL_INIT_NOPARACHUTE`: compatibility; this flag is ignored
  //!
  //! \throws a SDLInitFailedError if there was an error creating the
  //! SDL object.
  //!
  //! \return A new SDL object
  SDL(const std::shared_ptr<SDLWrapper> &sdl_wrapper_, Uint32 flags);

  //! SDL Destructor
  ~SDL();

  // Explicitly delete the generated default copy constructor we don't
  // want multiple copies floating around since SDL is a managed SDL
  // resource with expectations on single Init() and single Quit()
  // calls.
  SDL(const SDL &) = delete;

  // Explicitly delete the generated default copy assignment operator
  // we don't want multiple copies floating around since SDL is a
  // managed resource with expectations on single Init() and single
  // Quit() calls.
  SDL &operator=(const SDL &) = delete;

  // move constructor
  SDL(SDL &&) noexcept;

  // move assignment operator
  SDL &operator=(SDL &&) noexcept;

  // fowards to the SDLWrapper class

  //! Get a message about the last error that occurred on the current
  //! thread.
  //!
  //! \returns a message with information about the last error that
  //!          occurred or an empty string if no error has occured
  //!          since the last call to ClearError().
  //!          You should check the return values of SDL function
  //!          calls to determine when to call GetError().
  const char *GetError(void);

  //! Set the SDL error message for the current thread
  //!
  //! \param fmt a printf()-style message format string
  //! \param ... additional parameters for the format string
  //!
  //! \returns always -1
  int SetError(SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Log a message with SDL_LOG_CATEGORY_APPLICATION and SDL_LOG_PRIORITY_INFO
  //!
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  void Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Log a message with SDL_LOG_PRIORITY_INFO
  //!
  //! \param category The log category
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  void LogInfo(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Log a message with SDL_LOG_PRIORITY_ERROR
  //!
  //! \param category The log category
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  void LogError(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Set the priority of a particular log category
  //!
  //! \param category the category to assign a priority to.
  //! \param priority the SDL_LogPriority to assign.
  void LogSetPriority(int category, SDL_LogPriority priority);

  //! Create an SDL_Window
  //!
  //! Create an SDL_Window with the given parameters.
  //! Users should use the SDLWindow class, not this one, to create
  //! windows.
  //!
  //! \param title The title of the window
  //! \param x The x position of the window
  //! \param y The y position of the window
  //! \param w The width of the window, in screen coordinates
  //! \param h The height of the window, in screen coordinates
  //! \param flags SDL_WindowFlags to change the behavior and appearance of the
  //!        created window.  The following flags can be OR'd together:
  //!        - `SDL_WINDOW_FULLSCREEN`: fullscreen window
  //!        - `SDL_WINDOW_FULLSCREEN_DESKTOP`: fullscreen window at desktop
  //!        resolution
  //!        - `SDL_WINDOW_OPENGL`: window usable with an OpenGL context
  //!        - `SDL_WINDOW_VULKAN`: window usable with a Vulkan instance
  //!        - `SDL_WINDOW_METAL`: window usable with a Metal instance
  //!        - `SDL_WINDOW_HIDDEN`: window is not visible
  //!        - `SDL_WINDOW_BORDERLESS`: no window decoration
  //!        - `SDL_WINDOW_RESIZABLE`: window can be resized
  //!        - `SDL_WINDOW_MINIMIZED`: window is minimized
  //!        - `SDL_WINDOW_MAXIMIZED`: window is maximized
  //!        - `SDL_WINDOW_INPUT_GRABBED`: window has grabbed input focus
  //!        - `SDL_WINDOW_ALLOW_HIGHDPI`: window should be created in high-DPI
  //!        mode if
  //!          supported (>= SDL 2.0.1)
  //!
  //! \returns The new SDL_Window
  SDL_Window *SDLCALL CreateWindow(const char *title, int x, int y, int w,
                                   int h, Uint32 flags);

  //! Destroy a window
  //!
  //! \param window the SDL_Window to destroy
  void DestroyWindow(SDL_Window *window);

  //! Create an OpenGL context for an OpenGL window and make it
  //! current.
  //!
  //! \param window the window to associate with the context.
  //!
  //! \returns the OpenGL context associated with the window, or NULL
  //!          on error.
  SDL_GLContext GL_CreateContext(SDL_Window *window);

  //! Delete an OpenGL context.
  //!
  //! \param context the OpenGL context to be deleted.
  void GL_DeleteContext(SDL_GLContext context);

  //! Set up an OpenGL context for rendering into an OpenGL Window
  //!
  //! \param window the window to associate with the context
  //! \param context the OpenGL context to associate with the window
  //!
  //! \returns 0 on success or a negative error code on failure.
  int GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);

  //! Get the size of a window's underlying drawable in pixels
  //!
  //! \param w a pointer to a variable for storing the width in pixels
  //! \param h a pointer to a variable for storing the height in pixels
  void GL_GetDrawableSize(SDL_Window *window, int *w, int *h);

  //! Update a window with OpenGL rendering.
  //!
  //! \param window the window to change.
  void GL_SwapWindow(SDL_Window *window);

  //! Get the swap interval for the current OpenGL context.
  //!
  //! \returns 0 if there is no vertical retrace synchronization, 1 if
  //!          the buffer swap is synchronized with the vertical
  //!          retrace, and -1 if late swaps happen immediately
  //!          instead of waiting for the next retrace
  int GL_GetSwapInterval(void);

  //! Get information about the current display mode.
  //!
  //! \param displayIndex the index of the display to query.
  //! \param mode an SDL_DisplayMode structure filled in with the
  //!        current display mode.
  //!
  //! \returns 0 on success or a negative error code on failure
  int GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode);

  //! Cleanup SDL
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  bool is_in_unspecified_state() const override;

private:
  //! An object that holds pointers to the underlying SDL functions
  std::shared_ptr<SDLWrapper> sdl_wrapper = nullptr;

  //! Constructor helper function that actually builds the SDL object
  void build(Uint32 flags);

  //! initialized indicates whether SDL has been initialized
  bool initialized = false;
};

} // namespace sdl_opengl_cpp

#endif
