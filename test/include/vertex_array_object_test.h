#ifndef _SDL_OPENGL_CPP_VERTEX_ARRAY_OBJECT_TEST_H_
#define _SDL_OPENGL_CPP_VERTEX_ARRAY_OBJECT_TEST_H_

#include <optional>

namespace sdl_opengl_cpp {

class VertexArrayObjectTester {
public:
  VertexArrayObjectTester(const std::shared_ptr<GLContext> &ctx);

  // Explicitly delete the generated default copy constructor
  VertexArrayObjectTester(const VertexArrayObjectTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  VertexArrayObjectTester &operator=(const VertexArrayObjectTester &) = delete;

  // move constructor
  VertexArrayObjectTester(VertexArrayObjectTester &&) = delete;

  // move assignment operator
  VertexArrayObjectTester &operator=(VertexArrayObjectTester &&) = delete;

  // Surface the underlying OpenGL Vertex Array Object "name"
  // (handle or id) so we can test it.
  GLuint VAO();

  void set_invalid_VAO();

public:
  std::optional<VertexArrayObject> vao = std::nullopt;
};

} // namespace sdl_opengl_cpp

#endif
