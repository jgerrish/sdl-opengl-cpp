#include "SDL_opengl.h"
#include <SDL.h>

#include "gl_context.h"
#include "opengl.h"

using namespace sdl_opengl_cpp;

void GLContext::glClear(GLbitfield mask) { return gl_context->glClear(mask); }

GLenum GLContext::glGetError() { return gl_context->glGetError(); }

void GLContext::glFlush() { return gl_context->glFlush(); }

void GLContext::glEnableClientState(GLenum array) {
  return gl_context->glEnableClientState(array);
}

void GLContext::glDisableClientState(GLenum array) {
  return gl_context->glDisableClientState(array);
}

void GLContext::glDrawArrays(GLenum mode, GLint first, GLsizei count) {
  return gl_context->glDrawArrays(mode, first, count);
}

void GLContext::glVertexPointer(GLint size, GLenum type, GLsizei stride,
                                const GLvoid *pointer) {
  return gl_context->glVertexPointer(size, type, stride, pointer);
}

// Uniform functions
void GLContext::glUniform3fv(GLint location, GLsizei count,
                             const GLfloat *value) {
  return gl_context->glUniform3fv(location, count, value);
}

void GLContext::glUniformMatrix4fv(GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
  return gl_context->glUniformMatrix4fv(location, count, transpose, value);
}

void GLContext::glGenBuffers(GLsizei n, GLuint *buffers) {
  return gl_context->glGenBuffers(n, buffers);
}

void GLContext::glBindBuffer(GLenum target, GLuint buffer) {
  return gl_context->glBindBuffer(target, buffer);
}

void GLContext::glBufferData(GLenum target, GLsizeiptr size, const void *data,
                             GLenum usage) {
  return gl_context->glBufferData(target, size, data, usage);
}

void GLContext::glDeleteBuffers(GLsizei n, const GLuint *buffers) {
  return gl_context->glDeleteBuffers(n, buffers);
}

void GLContext::glGenVertexArrays(GLsizei n, GLuint *arrays) {
  return gl_context->glGenVertexArrays(n, arrays);
}

void GLContext::glBindVertexArray(GLuint array) {
  return gl_context->glBindVertexArray(array);
}

void GLContext::glEnableVertexAttribArray(GLuint index) {
  return gl_context->glEnableVertexAttribArray(index);
}

void GLContext::glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                      GLboolean normalized, GLsizei stride,
                                      const void *pointer) {
  return gl_context->glVertexAttribPointer(index, size, type, normalized,
                                           stride, pointer);
}

void GLContext::glDeleteVertexArrays(GLsizei n, const GLuint *arrays) {
  return gl_context->glDeleteVertexArrays(n, arrays);
}

GLuint GLContext::glCreateShader(GLenum type) {
  return gl_context->glCreateShader(type);
}

void GLContext::glShaderSource(GLuint shader, GLsizei count,
                               const GLchar *const *string,
                               const GLint *length) {
  return gl_context->glShaderSource(shader, count, string, length);
}

void GLContext::glCompileShader(GLuint shader) {
  return gl_context->glCompileShader(shader);
}

void GLContext::glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
  return gl_context->glGetShaderiv(shader, pname, params);
}

void GLContext::glGetShaderInfoLog(GLuint program, GLsizei bufSize,
                                   GLsizei *length, GLchar *infoLog) {
  return gl_context->glGetShaderInfoLog(program, bufSize, length, infoLog);
}

void GLContext::glDeleteShader(GLuint shader) {
  return gl_context->glDeleteShader(shader);
}

GLuint GLContext::glCreateProgram() { return gl_context->glCreateProgram(); }

void GLContext::glLinkProgram(GLuint program) {
  return gl_context->glLinkProgram(program);
}

GLuint GLContext::glAttachShader(GLuint program, GLuint shader) {
  return gl_context->glAttachShader(program, shader);
}

void GLContext::glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
  return gl_context->glGetProgramiv(program, pname, params);
}

void GLContext::glGetProgramInfoLog(GLuint program, GLsizei bufSize,
                                    GLsizei *length, GLchar *infoLog) {
  return gl_context->glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void GLContext::glUseProgram(GLuint program) {
  return gl_context->glUseProgram(program);
}

GLint GLContext::glGetUniformLocation(GLuint program, const GLchar *name) {
  return gl_context->glGetUniformLocation(program, name);
}

void GLContext::glGetAttachedShaders(GLuint program, GLsizei maxCount,
                                     GLsizei *count, GLuint *shaders) {
  return gl_context->glGetAttachedShaders(program, maxCount, count, shaders);
}

void GLContext::glDeleteProgram(GLuint program) {
  return gl_context->glDeleteProgram(program);
}
