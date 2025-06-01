#ifndef _SDL_OPENGL_CPP_SHADER_TEST_H_
#define _SDL_OPENGL_CPP_SHADER_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"
#include "shader.h"

namespace sdl_opengl_cpp {

class ShaderTester {
public:
  ShaderTester(const std::shared_ptr<GLContext> &ctx);

  // Explicitly delete the generated default copy constructor
  ShaderTester(const ShaderTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  ShaderTester &operator=(const ShaderTester &) = delete;

  // move constructor
  ShaderTester(ShaderTester &&) = delete;

  // move assignment operator
  ShaderTester &operator=(ShaderTester &&) = delete;

  // Surface the underlying OpenGL Shader "name" (handle or id) so
  // we can test it.
  GLuint shader_handle();

  void set_invalid_shader();

public:
  std::optional<Shader> shader = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
