//! sdl_opengl.cpp
//!
//! Example code for creating and managing SDL OpenGL windows
//!
//! This file and the corresponding source file show how to use the
//! new SDL and SDLWindow classes for creating and using SDL OpenGL
//! features.
//!
//! sdl_opengl_tester.cpp contained a lot of this code but wasn't
//! testable on headless machines since there was no mocking for it.
//! It also wasn't very well organized.  This code isn't well
//! organized yet either but the SDL and SDLWindow classes will make
//! it easier to refactor in the next version.
//!
//! The custom deleters have mostly been removed and replaced with
//! destructors in the SDL and SDLWindow class.  Ownership of handles
//! is now more clear and hopefully will allow a safety refactoring.
//!
//! The next version of this file will abstract out more of the GL
//! context setup and provide only one or two public functions in this
//! class:
//!
//! A public init() and rungl() or run() that takes a user's
//! std::function function that should be called on initialization or
//! when the program has started running.  That std::function will
//! accept the SDLWindow or SDL object.
#ifndef _SDL_OPENGL_H_
#define _SDL_OPENGL_H_

#include <functional>
#include <memory>
#include <optional>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include <SDL.h>

#ifdef NO_EXCEPTIONS
#include "errors.h"
#else
#include "move_checker.h"
#include "spdlog/spdlog.h"
#endif

#include "clipping_planes.h"
#include "sdl_base.h"
#include "sdl_window.h"

#include "gl_context.h"
#include "opengl.h"

using namespace std;

namespace sdl_opengl_cpp {

// nested namespaces added in C++17
namespace sdl_opengl {

#ifndef NO_EXCEPTIONS

//! A LoadOpenGLContextError exception
//!
//! An error occurred trying to load an OpenGL Context
//!
class LoadOpenGLContextError : public runtime_error {
  // Inherit constructors from runtime_error
  using runtime_error::runtime_error;
};

#endif

} // namespace sdl_opengl

using namespace sdl_opengl;

#ifndef NO_EXCEPTIONS
class SDLOpenGL : private MoveChecker {
#else
class SDLOpenGL : public Errors {
#endif
public:
  SDLOpenGL(const std::shared_ptr<SDL> &sdl_,
            const ClippingPlanes &clipping_planes_);

  SDLOpenGL(const std::shared_ptr<SDL> &sdl_,
            const std::shared_ptr<GLContext> &ctx,
            const ClippingPlanes &clipping_planes_);

  SDLOpenGL(const std::shared_ptr<SDL> &sdl,
            std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window,
            const std::function<void(
                std::shared_ptr<GLContext> &context, std::shared_ptr<SDL> &s,
                std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window)>
                &func,
            const ClippingPlanes &clipping_planes_);
  ;

  ~SDLOpenGL();

  //! Cleanup the SDLOpenGL object
  //!
  //! This method handles everything the destructor would do, and is
  //! called directly by the destructor.  The class design is
  //! heavily RAII based and many errors are assumed unrecoverable,
  //! so cleanup() is called from multiple points.
  void cleanup() noexcept;

  // Explicitly delete the generated default copy constructor
  SDLOpenGL(const SDLOpenGL &) = delete;

  // Explicitly delete the generated default copy assignment operator
  SDLOpenGL &operator=(const SDLOpenGL &) = delete;

  // move constructor
  SDLOpenGL(SDLOpenGL &&) noexcept;

  // move assignment operator
  SDLOpenGL &operator=(SDLOpenGL &&) noexcept;

  // int init();

  //! Load the SDL context
  //!
  //! This function sets up the SDL function pointers using the
  //! SDL_PROC macros in SDL_glfuncs.h
  //!
  //! \returns 0 on success, -1 on failure
  int load_context();

  //! Start running the main application.  It loads the function
  //! pointers, sets up the GL context.
  //!
  //! \throws runtime_error on failure to create the GL context.
  //!
  //! \returns 0 on success, -1 on failure
  int rungl();

  //! Helper function for the SDLWindow constructor
  int rungl_with_window(
      const std::function<void(
          std::shared_ptr<GLContext> &gl_context, std::shared_ptr<SDL> &sdl,
          std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window)>
          &runner);

  //! Make this GL context the current one and set the viewport
  //!
  //! \returns 0 on success
  int make_current();

  //! Update the window with GL rendering
  //!
  //! \returns 0 on success
  int swap_window();

  //! Log the swap interval
  void LogSwapInterval();

  //! Set the rendering settings for OpenGL
  //!
  //! This sets things like the clipping planes and perspective
  //!
  //! In particular it calls glOrtho and sets the depth function and
  //! shade model.
  int set_rendering_settings();

  //! Determine if the class instance is in an unspecified state after
  //! a move.
  //!
  //! \returns true if the instance is in an unspecified state, false
  //! if it isn't
  bool is_in_unspecified_state() const override;

public:
  //! GLContext is a basic wrapper around the SDL GL_Context
  //! structure.  It creates a C++ class which lets us create mocks
  //! for OpenGL.  It is also expected to be the primary member
  //! variable to use when making OpenGL function calls that are not
  //! wrapped in a custom C++ class or member function.
  //!
  //! gl_context is a smart pointer that manages the primary
  //! GLContext.
  std::shared_ptr<GLContext> glcontext = nullptr;

private:
  //! Access to the SDL functions
  std::shared_ptr<SDL> sdl = nullptr;

  //! This is the structure that the SDL_PROC macro assigns functions
  //! to.  Each function is a field of gl_context.
  //!
  //! For example: gl_context.glBegin points to the glBegin function
  //! configured in SDL_glfuncs.h
  //!
  //! The LoadContext member function creates the SDL_PROC macro, and
  //! SDL_glfuncs.h uses the macro for each extern OpenGL function we
  //! need to use.
  GL_Context gl_context = {};

  //! sdl_gl_context is the SDL OpenGL context
  //!
  //! It is used for some SDL window and context management routines
  //! that would normally be handled by libraries such as GLUT or
  //! GLEW.  For example, SDL_GL_MakeCurrent.
  SDL_GLContext sdl_gl_context = nullptr;

  //! window is the main SDL_Window for this application
  //! Currently we only support one window per application
  std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> window = nullptr;

  //! The clipping planes to use for glOrtho
  ClippingPlanes clipping_planes;
};

} // namespace sdl_opengl_cpp

#endif
