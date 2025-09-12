#define DOCTEST_CONFIG_IMPLEMENT

#include <memory>
#include <string>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#include <doctest/doctest.h>

#define HAVE_OPENGL

#include "gl_context.h"

#include "sdl_opengl_tester.h"

#include "shader.h"
#include "vertex_array_object.h"
#include "vertex_buffer_object.h"

int main(int argc, char **argv) {

  doctest::Context context;
  context.applyCommandLine(argc, argv);

  int res = context.run(); // run doctest

  // important - query flags (and --exit) rely on the user doing this
  if (context.shouldExit()) {
    // propagate the result of the tests
    return res;
  }

  return res;
}

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_window;

SDLOpenGLTester::SDLOpenGLTester(const std::shared_ptr<SDL> &sdl_) : sdl(sdl_) {
  int res = rungl();

  if (res != 0) {
#ifndef NO_EXCEPTIONS
    throw SDLInitFailed("rungl failed");
#else
    CHECK(false);
    return;
#endif
  }
}

int SDLOpenGLTester::LoadContext() {
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

void SDLOpenGLTester::LogSwapInterval(void) {
#ifndef NO_EXCEPTIONS
  spdlog::info("Swap Interval : %d", sdl->sdl_wrapper->GL_GetSwapInterval());
#else
  sdl->sdl_wrapper->Log("Swap Interval : %d\n", SDL_GL_GetSwapInterval());
#endif
}

// Careful, these tests already use move constructors and assignment
// without explicitly testing them

// A lot of this needs to be abstracted into a common class that can
// be initialized and easily used by library users.
int SDLOpenGLTester::rungl() {
  SDL_DisplayMode mode;

  /* Enable standard application logging */
  // sdl->sdl_wrapper->LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
  // SDL_LOG_PRIORITY_INFO);

  window = std::make_unique<SDLWindow>(
      SDLWindow(sdl, "SDLOpenGLTester", 0, 0, 640, 480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

  /* Create OpenGL context */
  sdl_gl_context = window->GL_CreateContext();

  if (!sdl_gl_context) {
#ifndef NO_EXCEPTIONS
    spdlog::error("SDL_GL_CreateContext(): {}", sdl->sdl_wrapper->GetError());
#endif
    // sdl->sdl_wrapper->LogError(SDL_LOG_CATEGORY_APPLICATION,
    // "SDL_GL_CreateContext(): %s\n",
    //              sdl->sdl_wrapper->GetError());
#ifndef NO_EXCEPTIONS
    throw runtime_error("Error creating SDL_GL context");
#else
    CHECK(false);
    return -1;
#endif
  }

  /* Important: call this *after* creating the context */
  if (LoadContext() < 0) {
#ifndef NO_EXCEPTIONS
    spdlog::error("Could not load GL functions");
    throw runtime_error("Error loading GL functions");
#else
    sdl->sdl_wrapper->LogError(SDL_LOG_CATEGORY_APPLICATION,
                               "Could not load GL functions\n");
    CHECK(false);
    return -1;
#endif
  }

  sdl->sdl_wrapper->GetCurrentDisplayMode(0, &mode);
#ifndef NO_EXCEPTIONS
  spdlog::info("Screen BPP    : %" SDL_PRIu32, SDL_BITSPERPIXEL(mode.format));
#else
  sdl->sdl_wrapper->Log("Screen BPP    : %" SDL_PRIu32, "\n",
                        SDL_BITSPERPIXEL(mode.format));
#endif

  LogSwapInterval();

  /* Set rendering settings */
  gl_context.glMatrixMode(GL_PROJECTION);
  gl_context.glLoadIdentity();
  gl_context.glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
  // gl_context.glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
  gl_context.glMatrixMode(GL_MODELVIEW);
  gl_context.glLoadIdentity();
  gl_context.glEnable(GL_DEPTH_TEST);
  gl_context.glDepthFunc(GL_LESS);
  gl_context.glShadeModel(GL_SMOOTH);

  /* Main render loop */
  then = sdl->sdl_wrapper->GetTicks();

  return 0;
}

SDLOpenGLTester::~SDLOpenGLTester() {
  if (sdl == nullptr) {
    return;
  }

  Uint32 now, frames = 0;
  /* Print out some timing information */
  now = sdl->sdl_wrapper->GetTicks();
  if (now > then) {
#ifndef NO_EXCEPTIONS
    spdlog::info("%2.2f frames per second",
                 (static_cast<double>(frames) * 1000) / (now - then));
#else
    sdl->sdl_wrapper->Log("%2.2f frames per second\n",
                          (static_cast<double>(frames) * 1000) / (now - then));
#endif
  }

  if (sdl_gl_context) {
    /* SDL_GL_MakeCurrent(0, NULL); */ /* doesn't do anything */
    sdl->sdl_wrapper->GL_DeleteContext(sdl_gl_context);
  }

  sdl->sdl_wrapper->Quit();
}

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

#include "mock_opengl.h"

#include "vertex_buffer_object_test.h"

// We have custom exceptions that are derived from runtime_error
// Since they are derived using "public" as the base class specifier, they
// are automatically translated by the doctest library.
//
// If they weren't public, we would need to register the custom
// exceptions with REGISTER_EXCEPTION_TRANSLATOR for doctest.
//
// Except what() would need to be some publicly accessible string
// method on our custom exception class.
//
// As an example, I've made GenBuffersError inherit from runtime_err as
// protected and exposed the what() method.  The following below shows how to
// register GenBuffersError for catching by doctest.
#ifndef NO_EXCEPTIONS
REGISTER_EXCEPTION_TRANSLATOR(vertex_buffer_object::GenBuffersError &ex) {
  return doctest::String(ex.what());
}
#endif

TEST_SUITE("sdl-opengl-cpp") {
  // TEST_CASE("Testing with a real SDL OpenGL Window") {
  // SDLOpenGLTester tester = {};

  // tester.rungl();

  //     CHECK(true == true);

  // #if GTEST_HAS_EXCEPTIONS

  // tester.cleanup();
  //   }
}
