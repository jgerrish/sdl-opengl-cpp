// Some of this documentation is copied from the SDL documentation
//
// This project, sdl-opengl-cpp, is license under the MIT License
// Copyright (c) 2025 Joshua Gerrish.
//
// The SDL license is included below for reference to the copied
// documentation.
//
// The copyright of that from the LICENSE.txt file as of 2025-10-08 is
// copied below:
//
// Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

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

  // SDL Surface functions

  //! Set an additional color value multiplied into blit operations
  //!
  //! When this surface is blitted, during the blit operation each source color
  //! channel is modulated by the appropriate color value according to the
  //! following formula:
  //!
  //! `srcC = srcC * (color / 255)`
  //!
  //! \param surface the SDL_Surface structure to update.
  //! \param r the red color value multiplied into blit operations.
  //! \param g the green color value multiplied into blit operations.
  //! \param b the blue color value multiplied into blit operations.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int SetSurfaceColorMod(SDL_Surface *surface, Uint8 r, Uint8 g,
                                 Uint8 b);

  //! Get the additional color value multiplied into blit operations.
  //!
  //! \param surface the SDL_Surface structure to query.
  //! \param r a pointer filled in with the current red color value.
  //! \param g a pointer filled in with the current green color value.
  //! \param b a pointer filled in with the current blue color value.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int GetSurfaceColorMod(SDL_Surface *surface, Uint8 *r, Uint8 *g,
                                 Uint8 *b);

  //! Set an additional alpha value used in blit operations.
  //!
  //! When this surface is blitted, during the blit operation the source alpha
  //! value is modulated by this alpha value according to the following formula:
  //!
  //! `srcA = srcA * (alpha / 255)`
  //!
  //! \param surface the SDL_Surface structure to update.
  //! \param alpha the alpha value multiplied into blit operations.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int SetSurfaceAlphaMod(SDL_Surface *surface, Uint8 alpha);

  //! Get the additional alpha value used in blit operations.
  //!
  //! \param surface the SDL_Surface structure to query.
  //! \param alpha a pointer filled in with the current alpha value.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int GetSurfaceAlphaMod(SDL_Surface *surface, Uint8 *alpha);

  //! Set the blend mode used for blit operations.
  //!
  //! To copy a surface to another surface (or texture) without blending with
  //! the existing data, the blendmode of the SOURCE surface should be set to
  //! `SDL_BLENDMODE_NONE`.
  //!
  //! \param surface the SDL_Surface structure to update.
  //! \param blendMode the SDL_BlendMode to use for blit blending.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int SetSurfaceBlendMode(SDL_Surface *surface,
                                  SDL_BlendMode blendMode);

  //! Get the blend mode used for blit operations.
  //!
  //! \param surface the SDL_Surface structure to query.
  //! \param blendMode a pointer filled in with the current SDL_BlendMode.
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int GetSurfaceBlendMode(SDL_Surface *surface,
                                  SDL_BlendMode *blendMode);

  //! Allocate a new RGB surface with a specific pixel format.
  //!
  //! This function operates mostly like SDL_CreateRGBSurface(),
  //! except instead of providing pixel color masks, you provide it
  //! with a predefined format from SDL_PixelFormatEnum.
  //!
  //! \param flags the flags are unused and should be set to 0.
  //! \param width the width of the surface.
  //! \param height the height of the surface.
  //! \param depth the depth of the surface in bits.
  //! \param format the SDL_PixelFormatEnum for the new surface's pixel format.
  //! \returns the new SDL_Surface structure that is created or NULL if it
  //! fails;
  //!          call SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual SDL_Surface *CreateRGBSurfaceWithFormat(Uint32 flags, int width,
                                                  int height, int depth,
                                                  Uint32 format);

  //! Free an RGB surface.
  //!
  //! It is safe to pass NULL to this function.
  //!
  //! \param surface the SDL_Surface to free.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual void FreeSurface(SDL_Surface *surface);

  //! Perform a fast blit from the source surface to the destination surface.
  //!
  //! SDL_UpperBlit() has been replaced by SDL_BlitSurface(), which is merely a
  //! macro for this function with a less confusing name.
  //!
  //! \param src the SDL_Surface structure to be copied from.
  //! \param srcrect the SDL_Rect structure representing the rectangle to be
  //!                copied, or NULL to copy the entire surface.
  //! \param dst the SDL_Surface structure that is the blit target.
  //! \param dstrect the SDL_Rect structure representing the rectangle that is
  //!                copied into.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int BlitSurface(SDL_Surface *src, const SDL_Rect *srcrect,
                          SDL_Surface *dst, SDL_Rect *dstrect);

  //! Save a surface to a seekable SDL data stream in BMP format.
  //!
  //! Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
  //! BMP directly. Other RGB formats with 8-bit or higher get converted to a
  //! 24-bit surface or, if they have an alpha mask or a colorkey, to a 32-bit
  //! surface before they are saved. YUV and paletted 1-bit and 4-bit formats
  //! are not supported.
  //!
  //! \param surface the SDL_Surface structure containing the image to be saved.
  //! \param filename the name of the file to save to
  //!
  //! \returns 0 on success or a negative error code on failure; call
  //!          SDL_GetError() for more information.
  //!
  //! Documentation copied from SDL_surface.h branch release-2.32.x on
  //! 2025-10-08
  virtual int SaveBMP(SDL_Surface *surface, const string &filename);
};

} // namespace sdl_opengl_cpp

#endif
