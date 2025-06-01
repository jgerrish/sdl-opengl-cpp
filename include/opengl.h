#ifndef _SDL_OPENGL_CPP_OPENGL_H_
#define _SDL_OPENGL_CPP_OPENGL_H_

struct GL_Context {
public:
#define SDL_PROC(ret, func, params) ret(APIENTRY *func) params;
#include "SDL_glfuncs.h"
#undef SDL_PROC
};

#endif
