#ifndef _SDL_OPENGL_CPP_PROGRAM_TEST_H_
#define _SDL_OPENGL_CPP_PROGRAM_TEST_H_

#include <deque>
#include <memory>
#include <optional>
#include <vector>

#include "gl_context.h"
#include "mock_opengl.h"
#include "program.h"

namespace sdl_opengl_cpp {

// small little class to help with testing
class UncompiledShader {
public:
  std::string name;
  std::string source;
  GLenum type;
};

class ProgramTester {
public:
  ProgramTester(const std::shared_ptr<GLContext> &ctx,
                const std::vector<UncompiledShader> &uncompiled_shaders);

  // Explicitly delete the generated default copy constructor
  ProgramTester(const ProgramTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  ProgramTester &operator=(const ProgramTester &) = delete;

  // move constructor
  ProgramTester(ProgramTester &&) = delete;

  // move assignment operator
  ProgramTester &operator=(ProgramTester &&) = delete;

  std::deque<std::unique_ptr<Shader>>
  createShaders(const std::shared_ptr<GLContext> &ctx,
                const std::vector<UncompiledShader> &uncompiled_shaders);

  // Surface the underlying OpenGL Program "name" (handle or id) so
  // we can test it.
  GLuint program_handle();

  void set_invalid_program();

public:
  std::optional<Program> program = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
