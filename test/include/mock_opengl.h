#ifndef _SDL_OPENGL_CPP_MOCK_OPENGL_H_
#define _SDL_OPENGL_CPP_MOCK_OPENGL_H_

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

#include "gl_context.h"

// gMock generates a bunch of warnings about mock methods that
// "should be initialized in the member initialization list"
// But #pragma GCC doesn't work with MSVC and I'm trying to
// push out an initial version that works to iterate on.
// #pragma GCC diagnostic ignored "-Weffc++"

namespace sdl_opengl_cpp {

class MockOpenGLContext : public GLContext {
public:
  MockOpenGLContext(std::shared_ptr<GL_Context> &ctx) : GLContext(ctx) {}

  ~MockOpenGLContext(){};

  // General functions

  MOCK_METHOD(void, glPushAttrib, (GLbitfield mask), (override));
  MOCK_METHOD(void, glPopAttrib, (), (override));

  MOCK_METHOD(GLenum, glGetError, (), (override));

  // Miscellaneous

  MOCK_METHOD(void, glBlendFunc, (GLenum sfactor, GLenum dfactor), (override));

  // Texture mapping
  MOCK_METHOD(void, glTexEnvf, (GLenum target, GLenum pname, GLfloat param),
              (override));

  MOCK_METHOD(void, glTexParameteri, (GLenum target, GLenum pname, GLint param),
              (override));

  MOCK_METHOD(void, glTexImage2D,
              (GLenum target, GLint level, GLint internalFormat, GLsizei width,
               GLsizei height, GLint border, GLenum format, GLenum type,
               const GLvoid *pixels),
              (override));

  // 1.1 functions
  MOCK_METHOD(void, glGenTextures, (GLsizei n, GLuint *textures), (override));
  MOCK_METHOD(void, glBindTexture, (GLenum target, GLuint texture), (override));
  MOCK_METHOD(void, glPushMatrix, (), (override));
  MOCK_METHOD(void, glPopMatrix, (), (override));
  MOCK_METHOD(void, glViewport,
              (GLint x, GLint y, GLsizei width, GLsizei height), (override));

  // Virtual Buffer Object functions
  MOCK_METHOD(void, glGenBuffers, (GLsizei n, GLuint *buffers), (override));
  MOCK_METHOD(void, glBindBuffer, (GLenum target, GLuint buffer), (override));
  MOCK_METHOD(void, glBufferData,
              (GLenum target, GLsizeiptr size, const void *data, GLenum usage),
              (override));
  MOCK_METHOD(void, glDeleteBuffers, (GLsizei n, const GLuint *buffers),
              (override));

  // Virtual Array Object functions
  MOCK_METHOD(void, glGenVertexArrays, (GLsizei n, GLuint *arrays), (override));
  MOCK_METHOD(void, glBindVertexArray, (GLuint array), (override));
  MOCK_METHOD(void, glEnableVertexAttribArray, (GLuint index), (override));
  MOCK_METHOD(void, glVertexAttribPointer,
              (GLuint index, GLint size, GLenum type, GLboolean normalized,
               GLsizei stride, const void *pointer),
              (override));
  MOCK_METHOD(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays),
              (override));

  MOCK_METHOD(GLuint, glCreateShader, (GLenum type), (override));
  MOCK_METHOD(void, glShaderSource,
              (GLuint shader, GLsizei count, const GLchar *const *string,
               const GLint *length),
              (override));
  MOCK_METHOD(void, glCompileShader, (GLuint shader), (override));
  MOCK_METHOD(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint *params),
              (override));
  MOCK_METHOD(void, glGetShaderInfoLog,
              (GLuint program, GLsizei bufSize, GLsizei *length,
               GLchar *infoLog),
              (override));
  MOCK_METHOD(void, glDeleteShader, (GLuint shader), (override));

  // Program functions
  MOCK_METHOD(GLuint, glCreateProgram, (), (override));
  MOCK_METHOD(GLuint, glAttachShader, (GLuint program, GLuint shader),
              (override));
  MOCK_METHOD(void, glLinkProgram, (GLuint program), (override));
  MOCK_METHOD(void, glGetProgramiv,
              (GLuint program, GLenum pname, GLint *params), (override));
  MOCK_METHOD(void, glGetProgramInfoLog,
              (GLuint program, GLsizei bufSize, GLsizei *length,
               GLchar *infoLog),
              (override));
  MOCK_METHOD(void, glUseProgram, (GLuint program), (override));
  MOCK_METHOD(GLint, glGetUniformLocation, (GLuint program, const GLchar *name),
              (override));
  MOCK_METHOD(void, glGetAttachedShaders,
              (GLuint program, GLsizei maxCount, GLsizei *count,
               GLuint *shaders),
              (override));
  MOCK_METHOD(void, glDeleteProgram, (GLuint program), (override));

  MOCK_METHOD(void, glDisable, (GLenum cap), (override));
};

} // namespace sdl_opengl_cpp

#endif
