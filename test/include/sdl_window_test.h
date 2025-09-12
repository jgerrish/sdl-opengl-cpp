#ifndef _SDL_OPENGL_CPP_SDL_WINDOW_TEST_H_
#define _SDL_OPENGL_CPP_SDL_WINDOW_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"
#include "sdl_window.h"
#include "sdl_wrapper.h"

namespace sdl_opengl_cpp {

class SDLWindowTester {
public:
  SDLWindowTester(const std::shared_ptr<SDL> &sdl_);

  // ~SDLWindowTester();

  // Explicitly delete the generated default copy constructor
  SDLWindowTester(const SDLWindowTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLWindowTester &operator=(const SDLWindowTester &) = delete;

  // move constructor
  SDLWindowTester(SDLWindowTester &&) = delete;

  // move assignment operator
  SDLWindowTester &operator=(SDLWindowTester &&) = delete;

  // Surface the underlying SDLWindow handle so we can test it.
  SDL_Window *sdl_window_handle();

  void set_invalid_sdl_window();

public:
  std::optional<sdl_window::SDLWindow> sdl_window = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
