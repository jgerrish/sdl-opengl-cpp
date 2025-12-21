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
  SDLWindowCreateOpenGLContext,

  // SDL Surface errors
  SDLSurfaceCreationError,
  SDLSurfaceLoadTextureError,

  // These are errors that belong to other modules, such as SDL TTF.
  //
  // We should probably have a hierarchical class-based error system for
  // those who disable exceptions.
  // Then I can move these into their relevant modules.  This design
  // breaks encapsulation.  But it is simple.
  //
  // For now, KISS
  //
  // If you have a favorite design that makes life easier, let me
  // know.

  // General SDL TTF errors
  SDLTTFInitFailedError,

  // An error opening a font
  SDLTTFFontOpenError,
};

extern std::ostream &operator<<(std::ostream &os, const error &e);
extern std::string error_as_string(const error &e);

} // namespace sdl_opengl_cpp

#endif

#endif
