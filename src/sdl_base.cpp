#include <string>

#include <stdarg.h>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"
#include "sdl_base.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl;

SDL::SDL() {
  sdl_wrapper = std::make_shared<SDLWrapper>();

  build(0);
}

SDL::SDL(Uint32 flags) {
  sdl_wrapper = std::make_shared<SDLWrapper>();

  build(flags);
}

SDL::SDL(const std::shared_ptr<SDLWrapper> &sdl_wrapper_)
    : sdl_wrapper{sdl_wrapper_} {
  build(0);
}

SDL::SDL(const std::shared_ptr<SDLWrapper> &sdl_wrapper_, Uint32 flags)
    : sdl_wrapper{sdl_wrapper_} {
  build(flags);
}

void SDL::build(Uint32 flags) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  int res = sdl_wrapper->Init(flags);

  if (res < 0) {
    const char *sdl_error_string = sdl_wrapper->GetError();
    std::string error_string;
    if (sdl_error_string == nullptr) {
      error_string = std::string("Couldn't initialize SDL.  SDL_Init failed.");
    } else {
      std::string error_msg = std::string(sdl_error_string);
      error_string =
          std::string("Couldn't initialize SDL.  SDL_Init failed: ") +
          sdl_error_string;
    }

#ifndef NO_EXCEPTIONS
    spdlog::error(error_string);
    throw SDLInitFailedError(error_string);
#else
    set_error(std::optional<sdl_opengl_cpp::error>(error::SDLInitFailedError));
    /* Enable standard application logging */
    sdl_wrapper->LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                                SDL_LOG_PRIORITY_INFO);

    sdl_wrapper->LogError(SDL_LOG_CATEGORY_APPLICATION, "%s",
                          error_string.c_str());

    return;
#endif
  } else {
    initialized = true;
  }
}

SDL::~SDL() { cleanup(); }

// move constructor
SDL::SDL(SDL &&s) noexcept {
  sdl_wrapper = s.sdl_wrapper;
  initialized = s.initialized;

#ifdef NO_EXCEPTIONS
  last_operation_failed = s.last_operation_failed;
  last_error = s.last_error;
#endif

  s.sdl_wrapper = nullptr;
  s.initialized = false;
}

// move assignment operator
SDL &SDL::operator=(SDL &&s) noexcept {
  if (&s != this) {
    sdl_wrapper = s.sdl_wrapper;
    initialized = s.initialized;
#ifdef NO_EXCEPTIONS
    last_operation_failed = s.last_operation_failed;
    last_error = s.last_error;
#endif
    s.sdl_wrapper = nullptr;
    s.initialized = false;
  }

  return *this;
}

const char *SDL::GetError(void) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return nullptr;
#endif
  }

  return sdl_wrapper->GetError();
}

int SDL::SetError(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return -1;
#endif
  }

  va_list args;

  va_start(args, fmt);
  int res = sdl_wrapper->SetError(fmt, args);
  va_end(args);

  return res;
}

void SDL::Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  va_list args;

  va_start(args, fmt);
  sdl_wrapper->Log(fmt, args);
  va_end(args);
}

void SDL::LogInfo(int category, SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  va_list args;

  va_start(args, fmt);
  sdl_wrapper->LogInfo(category, fmt, args);
  va_end(args);
}

void SDL::LogError(int category, SDL_PRINTF_FORMAT_STRING const char *fmt,
                   ...) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  va_list args;

  va_start(args, fmt);
  sdl_wrapper->LogError(category, fmt, args);
  va_end(args);
}

void SDL::LogSetPriority(int category, SDL_LogPriority priority) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  return sdl_wrapper->LogSetPriority(category, priority);
}

SDL_Window *SDL::CreateWindow(const char *title, int x, int y, int w, int h,
                              Uint32 flags) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return nullptr;
#endif
  }

  return sdl_wrapper->CreateWindow(title, x, y, w, h, flags);
}

void SDL::DestroyWindow(SDL_Window *window) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  return sdl_wrapper->DestroyWindow(window);
}

SDL_GLContext SDL::GL_CreateContext(SDL_Window *window) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return nullptr;
#endif
  }

  return sdl_wrapper->GL_CreateContext(window);
}

void SDL::GL_DeleteContext(SDL_GLContext context) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  return sdl_wrapper->GL_DeleteContext(context);
}

int SDL::GL_MakeCurrent(SDL_Window *window, SDL_GLContext context) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return -1;
#endif
  }

  return sdl_wrapper->GL_MakeCurrent(window, context);
}

void SDL::GL_GetDrawableSize(SDL_Window *window, int *w, int *h) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  return sdl_wrapper->GL_GetDrawableSize(window, w, h);
}

void SDL::GL_SwapWindow(SDL_Window *window) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return;
#endif
  }

  return sdl_wrapper->GL_SwapWindow(window);
}

int SDL::GL_GetSwapInterval(void) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return 0;
#endif
  }

  return sdl_wrapper->GL_GetSwapInterval();
}

int SDL::GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode) {
  if (is_in_unspecified_state()) {
#ifndef NO_EXCEPTIONS
    throw sdl::UnspecifiedStateError(
        "SDL Object is in an unspecified state");
#else
    last_operation_failed = true;
    last_error.emplace(sdl_opengl_cpp::error::UnspecifiedStateError);
    return -1;
#endif
  }

  return sdl_wrapper->GetCurrentDisplayMode(displayIndex, mode);
}

void SDL::cleanup() noexcept {
  if (initialized) {
    sdl_wrapper->Quit();
  }
  initialized = false;
}

bool SDL::is_in_unspecified_state() {
  if (sdl_wrapper == nullptr)
    return true;
  else
    return false;
}
