#ifndef _SDL_OPENGL_CPP_SDL_OPENGL_TEST_H_
#define _SDL_OPENGL_CPP_SDL_OPENGL_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"
#include "sdl_base.h"
#include "sdl_opengl_runner.h"

namespace sdl_opengl_cpp {

class SDLOpenGLTester {
public:
  SDLOpenGLTester(const std::shared_ptr<SDL> &sdl,
                  const std::shared_ptr<GLContext> &ctx);

  // Explicitly delete the generated default copy constructor
  SDLOpenGLTester(const SDLOpenGLTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLOpenGLTester &operator=(const SDLOpenGLTester &) = delete;

  // move constructor
  SDLOpenGLTester(SDLOpenGLTester &&) = delete;

  // move assignment operator
  SDLOpenGLTester &operator=(SDLOpenGLTester &&) = delete;

public:
  std::optional<SDLOpenGL> sdl_opengl = std::nullopt;
};

// Maybe make a member function
extern void sdl_opengl_constructor_works_with_exceptions_expectations(
    std::shared_ptr<MockOpenGLContext> &mock_opengl_context, GLuint &buffer,
    GLuint &first_available_buffer);

} // namespace sdl_opengl_cpp

#endif
