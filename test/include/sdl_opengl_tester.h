#ifndef _SDL_OPENGL_CPP_TEST_H_
#define _SDL_OPENGL_CPP_TEST_H_

#include <memory>
#include <optional>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include <SDL.h>

#include "sdl.h"
#include "sdl_window.h"

#include "opengl.h"

using namespace std;

namespace sdl_opengl_cpp {

class SDLInitFailed : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

class SDLOpenGLTester {
public:
  SDLOpenGLTester(const std::shared_ptr<SDL> &sdl_);
  ~SDLOpenGLTester();

  // Explicitly delete the generated default copy constructor
  SDLOpenGLTester(const SDLOpenGLTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLOpenGLTester &operator=(const SDLOpenGLTester &) = delete;

  SDLOpenGLTester(SDLOpenGLTester &&) = delete;

  SDLOpenGLTester &operator=(SDLOpenGLTester &&) = delete;

  int LoadContext();
  void LogSwapInterval(void);

  int rungl();

public:
  GL_Context gl_context = {};

private:
  const std::shared_ptr<SDL> sdl = nullptr;
  std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> window = nullptr;
  Uint32 then = 0;
  SDL_GLContext sdl_gl_context = nullptr;
};

} // namespace sdl_opengl_cpp

#endif
