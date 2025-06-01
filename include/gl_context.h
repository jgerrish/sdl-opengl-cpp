#ifndef _SDL_OPENGL_CPP_GL_CONTEXT_H_
#define _SDL_OPENGL_CPP_GL_CONTEXT_H_

#include <memory>

#include "SDL_opengl.h"
#include <SDL.h>

#include "opengl.h"

namespace sdl_opengl_cpp {
// gMock (google-mock, googlemock) doesn't allow testing directly on free
// functions. (From the gMock cook book (gmock_cook_book.md):
//
//   ### Mocking Free Functions
//
//   It is not possible to directly mock a free function (i.e. a
//   C-style function or a static method). If you need to, you can
//   rewrite your code to use an interface (abstract class).
//
// We're doing an adapter for the OpenGL class instead of a full
// macro expansion.  There might be some cleverness with writable
// RTTI or templates I could use to rewrite the struct or whatever,
// but this is basic and it works for now.
//
// Yes, it adds an extra layer to OpenGL calls.  If you have a
// suggestion for a portable and clean solution, let me know.
//
// Right now, there are not tests and wrappers for everything and
// most documentation is not included.  In particular, some of the
// uniform functions in here are used by my own downstream
// applications but don't have full C++ wrapper classes yet.
//
//
// For documentation, I'd recommend:
//
//   The OpenGL Programming Guide by Dave Shreiner, Graham Sellers,
//   John Kessenich, and Bill Licea-Kane. Eighth Edition or newer.
//
//   The OpenGL SuperBible by Graham Sellers, Richard S. Wright,
//   Jr. and Nicholas Haemel.  Seventh Edition or newer.
//
//   The OpenGL Graphics System A Specification (Version 4.1 (Core
//   Profile) was used here but there are more recent ones.
class GLContext {
public:
  // GLContext() { };
  GLContext(const std::shared_ptr<GL_Context> &ctx) noexcept
      : gl_context{ctx} {};
  virtual ~GLContext(){};

  // General functions
  virtual void glClear(GLbitfield mask);

  //! For every error code except for GL_OUT_OF_MEMORY, if an error
  //! code was set by calling a function, that function has no
  //! effect.  For GL_OUT_OF_MEMORY, the result is undefined.
  //!
  //!
  //! Error codes:
  //! GL_INVALID_ENUM      The enum argument is out of range.
  //!
  //! GL_INVALID_VALUE     The numeric argument is out of range.
  //!
  //! GL_INVALID_OPERATION The operation is illegal in its current
  //!                      state.  This is set for any *Pointer
  //!                      command (like glVertexAttribPointer)
  //!                      specifying the location and organization
  //!                      of vertex array data while zero is bound
  //!                      to the ARRAY_BUFFER buffer object binding
  //!                      point and the pointer argument is not
  //!                      NULL.  OpenGL 4.1 (Core Profile)
  //!
  //! GL_OUT_OF_MEMORY     Not enough memory is left to execute the command.
  //!
  //! GL_NO_ERROR          No error has occurred.
  virtual GLenum glGetError();

  virtual void glFlush();

  virtual void glEnableClientState(GLenum array);
  virtual void glDisableClientState(GLenum array);
  virtual void glDrawArrays(GLenum mode, GLint first, GLsizei count);

  virtual void glVertexPointer(GLint size, GLenum type, GLsizei stride,
                               const GLvoid *pointer);

  // Uniform functions
  virtual void glUniform3fv(GLint location, GLsizei count,
                            const GLfloat *value);
  virtual void glUniformMatrix4fv(GLint location, GLsizei count,
                                  GLboolean transpose, const GLfloat *value);

  // Virtual Buffer Object functions
  virtual void glGenBuffers(GLsizei n, GLuint *buffers);
  virtual void glBindBuffer(GLenum target, GLuint buffer);
  virtual void glBufferData(GLenum target, GLsizeiptr size, const void *data,
                            GLenum usage);
  virtual void glDeleteBuffers(GLsizei n, const GLuint *buffers);

  // Virtual Array Object functions
  virtual void glGenVertexArrays(GLsizei n, GLuint *arrays);
  virtual void glBindVertexArray(GLuint array);
  virtual void glEnableVertexAttribArray(GLuint index);

  //! parameters:
  //! GLuint index:         The shader attribute location
  //!
  //!                       It's 0 here for shader attribute in_Position:
  //!                       "layout(location=0) in vec3 in_Position;"
  //!
  //! GLint size:           The number of components to be updated per vertex,
  //!                       can be either 1, 2, 3, 4 or GL_BGRA
  //!
  //!                       It's 3 here to indicate a set of 3D vectors were
  //!                       used with x, y and z coordinates
  //!
  //! GLenum type:          the data type (GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
  //! GL_UNSIGNED_SHORT,
  //!                       GL_INT, GL_UNSIGNED_INT, GL_FIXED, GL_HALF_FLOAT,
  //!                       GL_FLOAT, or GL_DOUBLE.
  //!
  //!                       The data is an array of floats.
  //!
  //! GLboolean normalized: indicates that the vertex data should be normalized
  //! before being stored
  //!                       using the same method as glVertexAttribFourN*()
  //!
  //!                       The data is already normalized between 0 and 1.
  //!
  //! GLsizei stride:       The byte offset between consecutive elements in the
  //! array
  //!                       If stride is zero, the data is assumed to be tightly
  //!                       packed.
  //!
  //!                       This is 0 here because the data is a continuous
  //!                       array of floats tightly packed.  Each vertex of (x,
  //!                       y, z) coordinates is immediately followed by the
  //!                       next.
  //!
  //! const void *pointer:  The offset from the start of the buffer object
  //!                       (using zero-based addressing) for the first set of
  //!                       values in the array This is in bytes.
  virtual void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer);
  virtual void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);

  // Shader functions
  virtual GLuint glCreateShader(GLenum type);
  virtual void glShaderSource(GLuint shader, GLsizei count,
                              const GLchar *const *string, const GLint *length);
  virtual void glCompileShader(GLuint shader);
  virtual void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
  virtual void glGetShaderInfoLog(GLuint program, GLsizei bufSize,
                                  GLsizei *length, GLchar *infoLog);
  virtual void glDeleteShader(GLuint shader);

  // Program functions
  virtual GLuint glCreateProgram();
  virtual GLuint glAttachShader(GLuint program, GLuint shader);
  virtual void glLinkProgram(GLuint program);
  virtual void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
  virtual void glGetProgramInfoLog(GLuint program, GLsizei bufSize,
                                   GLsizei *length, GLchar *infoLog);
  virtual void glUseProgram(GLuint program);
  virtual GLint glGetUniformLocation(GLuint program, const GLchar *name);
  virtual void glGetAttachedShaders(GLuint program, GLsizei maxCount,
                                    GLsizei *count, GLuint *shaders);
  virtual void glDeleteProgram(GLuint program);

private:
  // The OpenGL context this program uses
  std::shared_ptr<GL_Context> gl_context = nullptr;
};

} // namespace sdl_opengl_cpp

#endif
