#ifndef _SDL_OPENGL_CPP_ERROR_H_
#define _SDL_OPENGL_CPP_ERROR_H_

#include <ostream>
#include <string>

#ifdef NO_EXCEPTIONS

namespace sdl_opengl_cpp {

enum class error {
  // General errors
  UnspecifiedStateError,

  // Errors related to OpenGL calls that haven't been mapped to custom
  // error types yet.
  InvalidOperationError,

  // Vertex Buffer Object errors
  BufferDataError,
  GenBuffersError,

  // Vertex Array Object errors
  GenVertexArraysError,

  // Shader errors
  ShaderCreationError,
  ShaderCompilationError,

  // Program errors
  ProgramCreationError,
  ProgramLinkingError,
  GetUniformLocationError,

  // General SDL errors
  SDLInitFailedError,
  SDLLoadOpenGLContext,

  // SDL Window errors
  SDLWindowCreationError,
  SDLWindowCreateOpenGLContext

};

extern std::ostream &operator<<(std::ostream &os, const error &e);
extern std::string error_as_string(const error &e);

} // namespace sdl_opengl_cpp

#endif

#endif
