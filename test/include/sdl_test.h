#ifndef _SDL_OPENGL_CPP_SDL_TEST_H_
#define _SDL_OPENGL_CPP_SDL_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"
#include "sdl.h"
#include "sdl_wrapper.h"

namespace sdl_opengl_cpp {

class SDLTester {
public:
  SDLTester(const std::shared_ptr<MockSDLWrapper> &mock_sdl_wrapper);

  // Explicitly delete the generated default copy constructor
  SDLTester(const SDLTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLTester &operator=(const SDLTester &) = delete;

  // move constructor
  SDLTester(SDLTester &&) = delete;

  // move assignment operator
  SDLTester &operator=(SDLTester &&) = delete;

  // Surface the underlying SDL handle so we can test it.
  bool initialized() const;

  void set_uninitialized_sdl();

public:
  std::optional<SDL> sdl = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
