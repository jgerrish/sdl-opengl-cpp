#ifndef _SDL_OPENGL_CPP_SDL_WRAPPER_H_
#define _SDL_OPENGL_CPP_SDL_WRAPPER_H_

#include <functional>
#include <memory>

#ifdef NO_EXCEPTIONS
#include <optional>
#else
#include "spdlog/spdlog.h"
#endif

#include <stdexcept>

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

// Microsoft has as global CreateWindow define
#undef CreateWindow

using namespace std;

namespace sdl_opengl_cpp {

class SDLWrapper {
public:
  SDLWrapper();
  SDLWrapper(Uint32 flags);

  virtual ~SDLWrapper();

  //! Initialize the SDL library
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
  //! \returns 0 on success or a negative error code on failure.
  virtual int Init(Uint32 flags);

  //! Clean up all the initiaized SDL subsystems.
  //!
  //! 
  virtual void Quit(void);

  //! Get a message about the last error that occurred on the current
  //! thread.
  //!
  //! \returns a message with information about the last error that
  //!          occurred or an empty string if no error has occured
  //!          since the last call to ClearError().
  //!          You should check the return values of SDL function
  //!          calls to determine when to call GetError().
  virtual const char *SDLCALL GetError(void);

  //! Set the SDL error message for the current thread
  //!
  //! \param fmt a printf()-style message format string
  //! \param ... additional parameters for the format string
  //!
  //! \returns always -1
  virtual int SetError(SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Delete an OpenGL context.
  //!
  //! \param context the OpenGL context to be deleted.
  virtual void GL_DeleteContext(SDL_GLContext context);

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
  virtual SDL_Window *SDLCALL CreateWindow(const char *title, int x, int y,
                                           int w, int h, Uint32 flags);

  //! Destroy a window
  //!
  //! \param window the SDL_Window to destroy
  virtual void DestroyWindow(SDL_Window *window);

  //! \returns 0 if there is no vertical retrace synchronization, 1 if
  //!          the buffer swap is synchronized with the vertical
  //!          retrace, and -1 if late swaps happen immediately
  //!          instead of waiting for the next retrace
  virtual int GL_GetSwapInterval(void);

  //! Log a message with SDL_LOG_CATEGORY_APPLICATION and SDL_LOG_PRIORITY_INFO
  //!
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  virtual void Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...);

  //! Log a message with SDL_LOG_PRIORITY_INFO
  //!
  //! \param category The log category
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  virtual void LogInfo(int category, SDL_PRINTF_FORMAT_STRING const char *fmt,
                       ...);

  //! Log a message with SDL_LOG_PRIORITY_ERROR
  //!
  //! \param category The log category
  //! \param fmt a printf() style message format string
  //! \param ... additional parameters for the format string
  virtual void LogError(int category, SDL_PRINTF_FORMAT_STRING const char *fmt,
                        ...);

  //! Set the priority of a particular log category
  //!
  //! \param category the category to assign a priority to.
  //! \param priority the SDL_LogPriority to assign.
  virtual void LogSetPriority(int category, SDL_LogPriority priority);

  //! Create an OpenGL context for an OpenGL window and make it
  //! current.
  //!
  //! \param window the window to associate with the context.
  //!
  //! \returns the OpenGL context associated with the window, or NULL
  //!          on error.
  virtual SDL_GLContext GL_CreateContext(SDL_Window *window);

  //! Get information about the current display mode.
  //!
  //! \param displayIndex the index of the display to query.
  //! \param mode an SDL_DisplayMode structure filled in with the
  //!        current display mode.
  //!
  //! \returns 0 on success or a negative error code on failure
  virtual int GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode);

  //! Set up an OpenGL context for rendering into an OpenGL Window
  //!
  //! \param window the window to associate with the context
  //! \param context the OpenGL context to associate with the window
  //!
  //! \returns 0 on success or a negative error code on failure.
  virtual int GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);

  //! Get the size of a window's underlying drawable in pixels
  //!
  //! \param w a pointer to a variable for storing the width in pixels
  //! \param h a pointer to a variable for storing the height in pixels
  virtual void GL_GetDrawableSize(SDL_Window *window, int *w, int *h);

  //! Update a window with OpenGL rendering.
  //!
  //! \param window the window to change.
  virtual void GL_SwapWindow(SDL_Window *window);

  //! Get the number of milliseconds since SDL library initialization.
  //!
  //! \returns an unsigned 32-bit value indiating the number of
  //!          milliseconds since the SDL library was initialized
  virtual Uint32 GetTicks(void);
};

} // namespace sdl_opengl_cpp

#endif
