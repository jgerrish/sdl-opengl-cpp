#include <doctest/doctest.h>
#include <memory>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "mock_sdl.h"
#include "opengl.h"
#include "sdl_window.h"
#include "sdl_window_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_window;

SDLWindowTester::SDLWindowTester(const std::shared_ptr<SDL> &sdl_) {
  sdl_window.emplace(SDLWindow(sdl_, "SDLWindowTester", 0, 0, 640, 480,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
}

SDL_Window *SDLWindowTester::sdl_window_handle() { return sdl_window->window; }

void SDLWindowTester::set_invalid_sdl_window() { sdl_window->window = nullptr; }

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDLWindow constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper,
              CreateWindow("SDLWindowTester", 0, 0, 640, 480,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
      .Times(1)
      // .Field(&MockSDL::initialized, testing::IsTrue())
      // SDL_Window is an opaque structure defined in SDL_Window in the
      // SDL include directory.  We cast a non-zero value to it which
      // lets us pass the nullptr test in the constructor.
      .WillOnce(testing::Return(reinterpret_cast<SDL_Window *>(1)));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  SDLWindowTester sdl_window_tester(sdl);
  EXPECT_CALL(*mock_sdl_wrapper,
              DestroyWindow(sdl_window_tester.sdl_window_handle()))
      .Times(1);

  CHECK(true);
}

#else

TEST_CASE(
    "testing that the SDLWindow constructor works with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper,
              CreateWindow("SDLWindowTester", 0, 0, 640, 480,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
      .Times(1)
      // SDL_Window is an opaque structure defined in SDL_Window in the
      // SDL include directory.  We cast a non-zero value to it which
      // lets us pass the nullptr test in the constructor.
      .WillOnce(testing::Return(reinterpret_cast<SDL_Window *>(1)));

  SDLWindowTester sdl_window_tester(sdl);

  EXPECT_CALL(*mock_sdl_wrapper,
              DestroyWindow(sdl_window_tester.sdl_window_handle()))
      .Times(1);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  CHECK(true);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDLWindow constructor throws an exception when "
          "CreateWindow fails with exceptions enabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper,
              CreateWindow("SDLWindowTester", 0, 0, 640, 480,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
      .Times(1)
      .WillOnce(testing::Return(reinterpret_cast<SDL_Window *>(NULL)));

  EXPECT_CALL(*mock_sdl_wrapper, DestroyWindow(_)).Times(0);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  CHECK_THROWS_WITH_AS([sdl] { SDLWindowTester sdl_window_tester(sdl); }(),
                       "ERROR::SDL_WINDOW::CREATE_SDL_WINDOW_FAILED",
                       SDLWindowCreationError);

  CHECK(true);
}

#else

TEST_CASE("testing that the SDLWindow constructor sets an error when "
          "CreateWindow fails with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper,
              CreateWindow("SDLWindowTester", 0, 0, 640, 480,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))
      .Times(1)
      .WillOnce(testing::Return(reinterpret_cast<SDL_Window *>(NULL)));

  SDLWindowTester sdl_window_tester(sdl);

  // Doesn't get called because SDL_Window pointer is null
  EXPECT_CALL(*mock_sdl_wrapper,
              DestroyWindow(sdl_window_tester.sdl_window_handle()))
      .Times(0);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  if (sdl_window_tester.sdl_window) {
    CHECK_EQ(sdl_window_tester.sdl_window->valid(), false);
    CHECK_EQ(sdl_window_tester.sdl_window->get_last_error(),
             sdl_opengl_cpp::error::SDLWindowCreationError);
  } else {
    CHECK(false);
  }

  CHECK_EQ(sdl_window_tester.sdl_window_handle(), nullptr);

  CHECK(true);
}

#endif
