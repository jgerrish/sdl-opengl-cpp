#ifndef _SDL_OPENGL_CPP_MOCK_SDL_H_
#define _SDL_OPENGL_CPP_MOCK_SDL_H_

#include <memory>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"
#include "sdl_base.h"
#include "sdl_wrapper.h"

#include "gl_context.h"

namespace sdl_opengl_cpp {

class MockSDLWrapper : public SDLWrapper {
public:
  MockSDLWrapper() : SDLWrapper() {}

  MockSDLWrapper(Uint32 flags) : SDLWrapper(flags) {}

  ~MockSDLWrapper(){};

  MOCK_METHOD(int, Init, (Uint32 flags), (override));
  MOCK_METHOD(void, Quit, (), (override));

  // Virtual Buffer Object functions
  MOCK_METHOD(const char *SDLCALL, GetError, (), (override));

  MOCK_METHOD(SDL_Window *SDLCALL, CreateWindow,
              (const char *title, int x, int y, int w, int h, Uint32 flags),
              (override));

  MOCK_METHOD(void, DestroyWindow, (SDL_Window * window), (override));

public:
  bool initialized = false;
};

} // namespace sdl_opengl_cpp

#endif
