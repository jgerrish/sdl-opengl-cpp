#ifndef _SDL_OPENGL_CPP_SDL_SURFACE_TEST_H_
#define _SDL_OPENGL_CPP_SDL_SURFACE_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"
#include "sdl_surface_base.h"
#include "sdl_wrapper.h"

namespace sdl_opengl_cpp {

class SDLSurfaceTester {
public:
  SDLSurfaceTester(const std::shared_ptr<SDL> &sdl_);

  // ~SDLSurfaceTester();

  // Explicitly delete the generated default copy constructor
  SDLSurfaceTester(const SDLSurfaceTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLSurfaceTester &operator=(const SDLSurfaceTester &) = delete;

  // move constructor
  SDLSurfaceTester(SDLSurfaceTester &&) = delete;

  // move assignment operator
  SDLSurfaceTester &operator=(SDLSurfaceTester &&) = delete;

  // Surface the underlying SDLSurface handle so we can test it.
  SDL_Surface *sdl_surface_handle();

  void set_invalid_sdl_surface();

public:
  std::optional<SDLSurface> sdl_surface = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
