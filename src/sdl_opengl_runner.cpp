#include <memory>
#include <string>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#define HAVE_OPENGL

#include "gl_context.h"
#include "sdl_opengl_runner.h"

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_opengl;
using namespace sdl_opengl_cpp::sdl_window;

SDLOpenGL::SDLOpenGL(const std::shared_ptr<SDL> &sdl_,
                     const ClippingPlanes &clipping_planes_)
    : sdl{sdl_}, clipping_planes{clipping_planes_} {
  window = std::make_unique<SDLWindow>(
      SDLWindow(sdl, "SDLOpenGLTester", 0, 0, 640, 480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

  int res = rungl();

  if (res != 0) {
#ifndef NO_EXCEPTIONS
    throw SDLInitFailedError("ERROR::SDL_OPENGL::SDL_INIT_FAILED");
#else
    set_error(std::optional<error>(sdl_opengl_cpp::error::SDLInitFailedError));
    return;
#endif
  }
}

SDLOpenGL::SDLOpenGL(const std::shared_ptr<SDL> &sdl_,
                     const std::shared_ptr<GLContext> &ctx,
                     const ClippingPlanes &clipping_planes_)
    : sdl{sdl_}, glcontext{ctx}, clipping_planes{clipping_planes_} {}

int SDLOpenGL::load_context() {
#ifdef SDL_VIDEO_DRIVER_UIKIT
#define __SDL_NOGETPROCADDR__
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
#define __SDL_NOGETPROCADDR__
#elif defined(SDL_VIDEO_DRIVER_PANDORA)
#define __SDL_NOGETPROCADDR__
#endif

// TODO: Get SDL_GL_GetProcAddress wrapped up somehow
#if defined __SDL_NOGETPROCADDR__
#define SDL_PROC(ret, func, params) gl_context.func = func;
#else
#define SDL_PROC(ret, func, params)                                            \
  do {                                                                         \
    gl_context.func =                                                          \
        reinterpret_cast<ret(*) params>(SDL_GL_GetProcAddress(#func));         \
    if (!gl_context.func) {                                                    \
      return SDL_SetError("Couldn't load GL function %s: %s", #func,           \
                          SDL_GetError());                                     \
    }                                                                          \
  } while (0);
#endif /* __SDL_NOGETPROCADDR__ */

#include "SDL_glfuncs.h"
#undef SDL_PROC
  return 0;
}

// TODO: Document move semantics of the window here and that after it is called
// it can't be used again.
SDLOpenGL::SDLOpenGL(
    const std::shared_ptr<SDL> &sdl_, std::unique_ptr<SDLWindow> &window_,
    const std::function<void(
        std::shared_ptr<GLContext> &context, std::shared_ptr<SDL> &s,
        std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window)> &func,
    const ClippingPlanes &clipping_planes_)
    : sdl{sdl_}, window{std::move(window_)}, clipping_planes{clipping_planes_} {
  // this.window = window_;
  // this.func = func_;

  int res = rungl_with_window(func);

  if (res != 0) {
#ifndef NO_EXCEPTIONS
    throw SDLInitFailedError("ERROR::SDL_OPENGL::SDL_INIT_FAILED");
#else
    set_error(std::optional<sdl_opengl_cpp::error>(error::SDLInitFailedError));
    return;
#endif
  }
}

void SDLOpenGL::LogSwapInterval() {
#ifndef NO_EXCEPTIONS
  spdlog::info("Swap Interval : %d", sdl->GL_GetSwapInterval());
#else
  sdl->Log("Swap Interval : %d\n", sdl->GL_GetSwapInterval());
#endif
}

// A lot of this needs to be abstracted into a common class that can
// be initialized and easily used by library users.
// This is being implemented in the simpler-library-init branch
// The next step is the ability for users to pass in their own
// std::function callbacks to use this library.
int SDLOpenGL::rungl() {
  SDL_DisplayMode mode;

  /* Create OpenGL context */
  sdl_gl_context = window->GL_CreateContext();

  if (!sdl_gl_context) {
#ifndef NO_EXCEPTIONS
    spdlog::error("SDL_GL_CreateContext(): {}", sdl->GetError());
    throw sdl_window::CreateOpenGLContextError("Error creating SDL_GL context");
#else
    sdl->LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): {}",
                  sdl->GetError());

    set_error(std::optional<sdl_opengl_cpp::error>(
        error::SDLWindowCreateOpenGLContext));
    return -1;
#endif
  }

  if (load_context() < 0) {
#ifndef NO_EXCEPTIONS
    spdlog::error("Could not load GL functions");
    throw sdl_opengl::LoadOpenGLContextError("Error loading GL functions");
#else
    sdl->Log("Could not load GL functions\n");
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::SDLLoadOpenGLContext));
    return -1;
#endif
  }

  std::shared_ptr<GL_Context> gl_ctx = std::make_shared<GL_Context>(gl_context);
  glcontext = std::make_shared<sdl_opengl_cpp::GLContext>(gl_ctx);

  sdl->GetCurrentDisplayMode(0, &mode);
#ifndef NO_EXCEPTIONS
  spdlog::info("Screen BPP    : %" SDL_PRIu32, SDL_BITSPERPIXEL(mode.format));
#else
  sdl->Log("Screen BPP    : %" SDL_PRIu32 "\n", SDL_BITSPERPIXEL(mode.format));
#endif

  LogSwapInterval();

  set_rendering_settings();

  return 0;
}

int SDLOpenGL::set_rendering_settings() {
  /* Set rendering settings */
  glcontext->glMatrixMode(GL_PROJECTION);
  glcontext->glLoadIdentity();
  glcontext->glOrtho(clipping_planes.left, clipping_planes.right,
                     clipping_planes.bottom, clipping_planes.top,
                     clipping_planes.near, clipping_planes.far);
  // gl_context->glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
  glcontext->glMatrixMode(GL_MODELVIEW);
  glcontext->glLoadIdentity();
  glcontext->glEnable(GL_DEPTH_TEST);
  glcontext->glDepthFunc(GL_LESS);
  glcontext->glShadeModel(GL_SMOOTH);

  return 0;
}

int SDLOpenGL::rungl_with_window(
    const std::function<
        void(std::shared_ptr<GLContext> &gl_context, std::shared_ptr<SDL> &s,
             std::unique_ptr<sdl_opengl_cpp::sdl_window::SDLWindow> &window)>
        &runner) {

  SDL_DisplayMode mode;

  /* Create OpenGL context */
  sdl_gl_context = window->GL_CreateContext();

  if (!sdl_gl_context) {
#ifndef NO_EXCEPTIONS
    spdlog::error("SDL_GL_CreateContext(): {}", sdl->GetError());
    throw sdl_window::CreateOpenGLContextError("Error creating SDL_GL context");
#else
    sdl->LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): {}",
                  sdl->GetError());
    set_error(std::optional<error>(
        sdl_opengl_cpp::error::SDLWindowCreateOpenGLContext));
    return -1;
#endif
  }

  /* Important: call this *after* creating the context */
  if (load_context() < 0) {
#ifndef NO_EXCEPTIONS
    spdlog::error("Could not load GL functions");
    throw sdl_opengl::LoadOpenGLContextError("Error loading GL functions");
#else
    sdl->Log("Could not load GL functions\n");
    set_error(
        std::optional<error>(sdl_opengl_cpp::error::SDLLoadOpenGLContext));
    return -1;
#endif
  }

  std::shared_ptr<GL_Context> gl_ctx = std::make_shared<GL_Context>(gl_context);
  glcontext = std::make_shared<sdl_opengl_cpp::GLContext>(gl_ctx);

  sdl->GetCurrentDisplayMode(0, &mode);
#ifndef NO_EXCEPTIONS
  spdlog::info("Screen BPP    : %" SDL_PRIu32, SDL_BITSPERPIXEL(mode.format));
#else
  sdl->Log("Screen BPP    : %" SDL_PRIu32 "\n", SDL_BITSPERPIXEL(mode.format));
#endif

  LogSwapInterval();

  set_rendering_settings();

  runner(glcontext, sdl, window);

  return 0;
}

int SDLOpenGL::make_current() {
  int w;
  int h;

  window->GL_MakeCurrent(sdl_gl_context);
  window->GL_GetDrawableSize(&w, &h);

  glcontext->glViewport(0, 0, w, h);

  return 0;
}

int SDLOpenGL::swap_window() {
  window->GL_SwapWindow();

  return 0;
}

SDLOpenGL::~SDLOpenGL() { cleanup(); }

void SDLOpenGL::cleanup() noexcept {
  if (sdl_gl_context) {
    /* SDL_GL_MakeCurrent(0, NULL); */ /* doesn't do anything */
    sdl->GL_DeleteContext(sdl_gl_context);
  }

  // sdl->sdl_wrapper->Quit();

  sdl_gl_context = nullptr;
  window = nullptr;
}

// move constructor
SDLOpenGL::SDLOpenGL(SDLOpenGL &&sgl) noexcept
    : clipping_planes{std::move(sgl.clipping_planes)} {
  gl_context = sgl.gl_context;
  sdl_gl_context = sgl.sdl_gl_context;
  window = std::move(sgl.window);

#ifdef NO_EXCEPTIONS
  last_operation_failed = sgl.last_operation_failed;
  last_error = sgl.last_error;
#endif

  sgl.sdl_gl_context = nullptr;
  sgl.window = nullptr;
}

// move assignment operator
SDLOpenGL &SDLOpenGL::operator=(SDLOpenGL &&sgl) noexcept {
  if (&sgl != this) {
    gl_context = sgl.gl_context;
    sdl_gl_context = sgl.sdl_gl_context;
    window = std::move(sgl.window);
    clipping_planes = sgl.clipping_planes;

#ifdef NO_EXCEPTIONS
    last_operation_failed = sgl.last_operation_failed;
    last_error = sgl.last_error;
#endif

    sgl.sdl_gl_context = nullptr;
    sgl.window = nullptr;
  }

  return *this;
}

bool SDLOpenGL::is_in_unspecified_state() const {
  if ((sdl_gl_context == nullptr) || (window == 0))
    return true;
  else
    return false;
}
