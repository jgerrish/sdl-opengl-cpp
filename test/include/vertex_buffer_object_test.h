#ifndef _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_TEST_H_
#define _SDL_OPENGL_CPP_VERTEX_BUFFER_OBJECT_TEST_H_

#include <memory>
#include <optional>

#include "mock_opengl.h"

namespace sdl_opengl_cpp {

class VertexBufferObjectTester {
public:
  VertexBufferObjectTester(const std::shared_ptr<GLContext> &ctx);

  // Explicitly delete the generated default copy constructor
  VertexBufferObjectTester(const VertexBufferObjectTester &) = delete;

  // Explicitly delete the generated default copy assignment operator
  VertexBufferObjectTester &
  operator=(const VertexBufferObjectTester &) = delete;

  // move constructor
  VertexBufferObjectTester(VertexBufferObjectTester &&) = delete;

  // move assignment operator
  VertexBufferObjectTester &operator=(VertexBufferObjectTester &&) = delete;

  // Surface the underlying OpenGL Vertex Buffer Object "name"
  // (handle or id) so we can test it.
  GLuint VBO();

  void set_invalid_VBO();

public:
  std::optional<VertexBufferObject> vbo = std::nullopt;
};

// Maybe make a member function
extern void vertex_buffer_constructor_works_with_exceptions_expectations(
    std::shared_ptr<MockOpenGLContext> &mock_opengl_context, GLuint &buffer,
    GLuint &first_available_buffer);

} // namespace sdl_opengl_cpp

#endif
