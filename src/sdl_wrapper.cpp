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

SDLWrapper::~SDLWrapper() {}

SDLWrapper::SDLWrapper() {}

SDLWrapper::SDLWrapper(Uint32 flags) {}

int SDLWrapper::Init(Uint32 flags) {
  int res = SDL_Init(flags);

  return res;
}

void SDLWrapper::Quit(void) { return SDL_Quit(); }

const char *SDLCALL SDLWrapper::GetError(void) { return SDL_GetError(); }

int SDLWrapper::SetError(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  int res = SDL_SetError(fmt, args);
  va_end(args);

  return res;
}

void SDLWrapper::GL_DeleteContext(SDL_GLContext context) {
  return SDL_GL_DeleteContext(context);
}

SDL_Window *SDLCALL SDLWrapper::CreateWindow(const char *title, int x, int y,
                                             int w, int h, Uint32 flags) {
  return SDL_CreateWindow(title, x, y, w, h, flags);
}

void SDLWrapper::DestroyWindow(SDL_Window *window) {
  return SDL_DestroyWindow(window);
}

int SDLWrapper::GL_GetSwapInterval(void) { return SDL_GL_GetSwapInterval(); }

void SDLWrapper::Log(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  SDL_Log(fmt, args);
  va_end(args);
}

void SDLWrapper::LogInfo(int category, SDL_PRINTF_FORMAT_STRING const char *fmt,
                         ...) {
  va_list args;

  va_start(args, fmt);
  SDL_LogInfo(category, fmt, args);
  va_end(args);
}

void SDLWrapper::LogError(int category,
                          SDL_PRINTF_FORMAT_STRING const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  SDL_LogError(category, fmt, args);
  va_end(args);
}

void SDLWrapper::LogSetPriority(int category, SDL_LogPriority priority) {
  return SDL_LogSetPriority(category, priority);
}

SDL_GLContext SDLWrapper::GL_CreateContext(SDL_Window *window) {
  return SDL_GL_CreateContext(window);
}

int SDLWrapper::GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode) {
  return SDL_GetCurrentDisplayMode(displayIndex, mode);
}

int SDLWrapper::GL_MakeCurrent(SDL_Window *window, SDL_GLContext context) {
  return SDL_GL_MakeCurrent(window, context);
}

void SDLWrapper::GL_GetDrawableSize(SDL_Window *window, int *w, int *h) {
  return SDL_GL_GetDrawableSize(window, w, h);
}

void SDLWrapper::GL_SwapWindow(SDL_Window *window) {
  return SDL_GL_SwapWindow(window);
}

Uint32 SDLWrapper::GetTicks(void) { return SDL_GetTicks(); }
