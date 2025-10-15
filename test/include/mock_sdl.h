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

  // Surface functions

  MOCK_METHOD(int, SetSurfaceColorMod,
              (SDL_Surface * surface, Uint8 r, Uint8 g, Uint8 b), (override));

  MOCK_METHOD(int, GetSurfaceColorMod,
              (SDL_Surface * surface, Uint8 *r, Uint8 *g, Uint8 *b),
              (override));

  MOCK_METHOD(int, SetSurfaceAlphaMod, (SDL_Surface * surface, Uint8 alpha),
              (override));

  MOCK_METHOD(int, GetSurfaceAlphaMod, (SDL_Surface * surface, Uint8 *alpha),
              (override));

  MOCK_METHOD(int, SetSurfaceBlendMode,
              (SDL_Surface * surface, SDL_BlendMode blendMode), (override));

  MOCK_METHOD(int, GetSurfaceBlendMode,
              (SDL_Surface * surface, SDL_BlendMode *blendMode), (override));

  MOCK_METHOD(SDL_Surface *, CreateRGBSurfaceWithFormat,
              (Uint32 flags, int width, int height, int depth, Uint32 format),
              (override));

  MOCK_METHOD(void, FreeSurface, (SDL_Surface * surface), (override));

  MOCK_METHOD(int, BlitSurface,
              (SDL_Surface * src, const SDL_Rect *srcrect, SDL_Surface *dst,
               SDL_Rect *dstrect),
              (override));

  MOCK_METHOD(int, SaveBMP,
              (SDL_Surface * surface, const std::string &filename), (override));

public:
  bool initialized = false;
};

} // namespace sdl_opengl_cpp

#endif
