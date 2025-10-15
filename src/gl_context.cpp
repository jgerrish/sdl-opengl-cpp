#include "SDL_opengl.h"
#include <SDL.h>

#include "gl_context.h"
#include "opengl.h"

using namespace sdl_opengl_cpp;

void GLContext::glPushAttrib(GLbitfield mask) {
  return gl_context->glPushAttrib(mask);
}

void GLContext::glPopAttrib() { return gl_context->glPopAttrib(); }

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

// Misc functions
// Needed for initialization

void GLContext::glMatrixMode(GLenum mode) {
  return gl_context->glMatrixMode(mode);
}

void GLContext::glLoadIdentity() { return gl_context->glLoadIdentity(); }

void GLContext::glOrtho(GLdouble left, GLdouble right, GLdouble bottom,
                        GLdouble top, GLdouble zNear, GLdouble zFar) {
  return gl_context->glOrtho(left, right, bottom, top, zNear, zFar);
}

void GLContext::glEnable(GLenum cap) { return gl_context->glEnable(cap); }

void GLContext::glDisable(GLenum cap) { return gl_context->glDisable(cap); }

void GLContext::glDepthFunc(GLenum func) {
  return gl_context->glDepthFunc(func);
}

void GLContext::glShadeModel(GLenum mode) {
  return gl_context->glShadeModel(mode);
}

void GLContext::glClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) {
  return gl_context->glClearColor(r, g, b, a);
}

void GLContext::glColor3fv(const GLfloat *color) {
  return gl_context->glColor3fv(color);
}

// Drawing Functions

void GLContext::glEnd() { return gl_context->glEnd(); }

void GLContext::glBegin(GLenum mode) { return gl_context->glBegin(mode); }

void GLContext::glTexCoord2f(GLfloat s, GLfloat t) {
  return gl_context->glTexCoord2f(s, t);
}

// Miscellaneous

void GLContext::glBlendFunc(GLenum sfactor, GLenum dfactor) {
  return gl_context->glBlendFunc(sfactor, dfactor);
}

// Texture mapping

void GLContext::glTexEnvf(GLenum target, GLenum pname, GLfloat param) {
  return gl_context->glTexEnvf(target, pname, param);
}

void GLContext::glTexParameteri(GLenum target, GLenum pname, GLint param) {
  return gl_context->glTexParameteri(target, pname, param);
}

void GLContext::glTexImage2D(GLenum target, GLint level, GLint internalFormat,
                             GLsizei width, GLsizei height, GLint border,
                             GLenum format, GLenum type, const GLvoid *pixels) {
  return gl_context->glTexImage2D(target, level, internalFormat, width, height,
                                  border, format, type, pixels);
}

// 1.1 functions

void GLContext::glGenTextures(GLsizei n, GLuint *textures) {
  return gl_context->glGenTextures(n, textures);
}

void GLContext::glBindTexture(GLenum target, GLuint texture) {
  return gl_context->glBindTexture(target, texture);
}

// Transformation

void GLContext::glPushMatrix() { return gl_context->glPushMatrix(); }

void GLContext::glPopMatrix() { return gl_context->glPopMatrix(); }

void GLContext::glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  return gl_context->glViewport(x, y, width, height);
}
