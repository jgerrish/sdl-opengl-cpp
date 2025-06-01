#define DOCTEST_CONFIG_IMPLEMENT

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

#include <doctest/doctest.h>

#define HAVE_OPENGL

#include "SDL_test_common.h"

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

  return -1;
}

using namespace sdl_opengl_cpp;

SDLOpenGLTester::SDLOpenGLTester() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    // We may lose stack trace and SDL error info if we fail on
    // building a string here
    string error_string =
        std::string("Couldn't initialize SDL.  SDL_Init failed: ") +
        std::string(SDL_GetError());

    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", error_string.c_str());
    spdlog::error(error_string);

    throw SDLInitFailed(error_string);
  }

  rungl();
}

int SDLOpenGLTester::LoadContext() {
#ifdef SDL_VIDEO_DRIVER_UIKIT
#define __SDL_NOGETPROCADDR__
#elif defined(SDL_VIDEO_DRIVER_ANDROID)
#define __SDL_NOGETPROCADDR__
#elif defined(SDL_VIDEO_DRIVER_PANDORA)
#define __SDL_NOGETPROCADDR__
#endif

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

static void LogSwapInterval(void);
static void LogSwapInterval(void) {
  SDL_Log("Swap Interval : %d\n", SDL_GL_GetSwapInterval());
}

void SDL_Window_Deleter::operator()(SDL_Window *window) {
  if (window == nullptr)
    return;

  SDL_DestroyWindow(window);

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "deleting window");
  // TODO Why is this crashing?  Lifetime of spdlog?  I'm curious.
  // concurrency bug?  It doesn't crash every run
  // It crashes supposedly on __atomic_load_n
  // I can't even figure out where that function lives.
  // stepping into it fails in gdb, maybe because it's a macro.
  // spdlog::info(log_start_del_msg);
  // // SPDLOG_INFO("Deleting SDL_Window");

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "deleted window");
  // TODO Why is this crashing?  I'm curious.
  // spdlog::info("Deleted SDL_Window");
  // spdlog::info(log_end_del_msg);
}

// Careful, these tests already use move constructors and assignment
// without explicitly testing them

// A lot of this needs to be abstracted into a common class that can
// be initialized and easily used by library users.
int SDLOpenGLTester::rungl() {
  SDL_DisplayMode mode;

  /* Enable standard application logging */
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

  window = std::unique_ptr<SDL_Window, SDL_Window_Deleter>(SDL_CreateWindow(
      "SDLOpenGLTester", 0, 0, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

  /* Create OpenGL context */
  sdl_gl_context = SDL_GL_CreateContext(window.get());

  if (!sdl_gl_context) {
    spdlog::error("SDL_GL_CreateContext(): {}\n", SDL_GetError());
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): %s\n",
                 SDL_GetError());
    throw runtime_error("Error creating SDL_GL context");
  }

  /* Important: call this *after* creating the context */
  if (LoadContext() < 0) {
    SDL_Log("Could not load GL functions\n");
    throw runtime_error("Error loading GL functions");
  }

  SDL_GetCurrentDisplayMode(0, &mode);
  SDL_Log("Screen BPP    : %" SDL_PRIu32 "\n", SDL_BITSPERPIXEL(mode.format));

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
  // frames = 0;
  then = SDL_GetTicks();

  return 0;
}

SDLOpenGLTester::~SDLOpenGLTester() {
  Uint32 now, frames = 0;
  /* Print out some timing information */
  now = SDL_GetTicks();
  if (now > then) {
    SDL_Log("%2.2f frames per second\n",
            (static_cast<double>(frames) * 1000) / (now - then));
  }

  if (sdl_gl_context) {
    /* SDL_GL_MakeCurrent(0, NULL); */ /* doesn't do anything */
    SDL_GL_DeleteContext(sdl_gl_context);
  }

  SDL_Quit();
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
