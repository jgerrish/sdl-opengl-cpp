#include <memory>

#include "sdl_window.h"

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_window;

SDLWindow::SDLWindow(const std::shared_ptr<SDL> &sdl_, const char *title, int x,
                     int y, int w, int h, Uint32 flags)
    : sdl{sdl_} {
  window = sdl->CreateWindow(title, x, y, w, h, flags);
  if (window == nullptr) {
#ifndef NO_EXCEPTIONS
    spdlog::error("ERROR::SDL_WINDOW::CREATE_SDL_WINDOW_FAILED");
    throw SDLWindowCreationError("ERROR::SDL_WINDOW::CREATE_SDL_WINDOW_FAILED");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::SDLWindowCreationError);
    cleanup();
    return;
#endif
  }
}

SDLWindow::~SDLWindow() { cleanup(); }

void SDLWindow::cleanup() noexcept {
  if (window == nullptr)
    return;

  sdl->DestroyWindow(window);
}

// move constructor
SDLWindow::SDLWindow(SDLWindow &&w) noexcept {
  sdl = w.sdl;
  window = w.window;

#ifdef NO_EXCEPTIONS
  last_operation_failed = w.last_operation_failed;
  last_error = w.last_error;
#endif

  w.sdl = nullptr;
  w.window = nullptr;
}

// move assignment operator
SDLWindow &SDLWindow::operator=(SDLWindow &&w) noexcept {
  if (&w != this) {
    sdl = w.sdl;
    window = w.window;
#ifdef NO_EXCEPTIONS
    last_operation_failed = w.last_operation_failed;
    last_error = w.last_error;
#endif
    w.sdl = nullptr;
    w.window = nullptr;
  }

  return *this;
}

bool SDLWindow::is_in_unspecified_state() {
  if ((sdl == nullptr) || (window == nullptr))
    return true;
  else
    return false;
}

// TODO: move tests
DECLSPEC SDL_GLContext SDLCALL SDLWindow::GL_CreateContext() {
  if ((sdl == nullptr) || (window == nullptr)) {
#ifndef NO_EXCEPTIONS
    throw SDLWindowUnspecifiedStateError(
        "SDLWindow is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return nullptr;
#endif
  }

  return sdl->GL_CreateContext(window);
}

// TODO: move tests
DECLSPEC int SDLCALL SDLWindow::GL_MakeCurrent(SDL_GLContext context) {
  if ((sdl == nullptr) || (window == nullptr)) {
#ifndef NO_EXCEPTIONS
    throw SDLWindowUnspecifiedStateError(
        "SDLWindow is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return -1;
#endif
  }

  return sdl->GL_MakeCurrent(window, context);
}

// TODO: move tests
DECLSPEC void SDLCALL SDLWindow::GL_GetDrawableSize(int *w, int *h) {
  if ((sdl == nullptr) || (window == nullptr)) {
#ifndef NO_EXCEPTIONS
    throw SDLWindowUnspecifiedStateError(
        "SDLWindow is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return;
#endif
  }

  sdl->GL_GetDrawableSize(window, w, h);
}

// TODO: move tests
DECLSPEC void SDLCALL SDLWindow::GL_SwapWindow() {
  if ((sdl == nullptr) || (window == nullptr)) {
#ifndef NO_EXCEPTIONS
    throw SDLWindowUnspecifiedStateError(
        "SDLWindow is in an unspecified state");
#else
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::UnspecifiedStateError));
    return;
#endif
  }

  sdl->GL_SwapWindow(window);
}
