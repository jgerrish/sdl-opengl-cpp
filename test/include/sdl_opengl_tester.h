#ifndef _SDL_OPENGL_CPP_TEST_H_
#define _SDL_OPENGL_CPP_TEST_H_

// extern static int LoadContext(GL_Context *data);

#include <memory>
#include <optional>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

using namespace std;

namespace sdl_opengl_cpp {

class SDLInitFailed : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

// We use a custom deleter class for our SDL_Window smart pointer.
class SDL_Window_Deleter {
public:
  void operator()(SDL_Window *window);

private:
  std::string log_start_del_msg{"Deleting SDL_Window"};
  std::string log_end_del_msg{"Deleted SDL_Window"};
};

class SDLOpenGLTester {
public:
  SDLOpenGLTester();

  ~SDLOpenGLTester();

  // Explicitly delete the generated default copy constructor
  SDLOpenGLTester(const SDLOpenGLTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLOpenGLTester &operator=(const SDLOpenGLTester &) = delete;

  SDLOpenGLTester(SDLOpenGLTester &&) = delete;

  SDLOpenGLTester &operator=(SDLOpenGLTester &&) = delete;

  int LoadContext();

  int rungl();

public:
  GL_Context gl_context = {};

private:
  std::unique_ptr<SDL_Window, SDL_Window_Deleter> window = nullptr;
  Uint32 then = 0;
  SDL_GLContext sdl_gl_context = nullptr;
};

} // namespace sdl_opengl_cpp

#endif
