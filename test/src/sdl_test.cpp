#include <doctest/doctest.h>
#include <memory>

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "mock_sdl.h"
#include "opengl.h"
#include "sdl_base.h"
#include "sdl_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_opengl_cpp;

SDLTester::SDLTester(const std::shared_ptr<MockSDLWrapper> &mock_sdl_wrapper) {
  sdl.emplace(SDL(mock_sdl_wrapper));
}

bool SDLTester::initialized() const { return sdl->initialized; }

void SDLTester::set_uninitialized_sdl() { sdl->initialized = false; }

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDL constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  SDLTester sdl_tester(mock_sdl_wrapper);

  CHECK(true);
}

#else

TEST_CASE("testing that the SDL constructor works with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  SDLTester sdl_tester(mock_sdl_wrapper);

  CHECK(true);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDL constructor throws an exception when "
          "Init fails with exceptions enabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0))
      .Times(1)
      .WillOnce(testing::Return(-1));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(0);

  const char *testing_init_error_msg = "testing init error msg";
  EXPECT_CALL(*mock_sdl_wrapper, GetError())
      .Times(1)
      .WillOnce(testing::Return(testing_init_error_msg));

  CHECK_THROWS_WITH_AS(
      [mock_sdl_wrapper] { SDLTester sdl_tester(mock_sdl_wrapper); }(),
      "Couldn't initialize SDL.  SDL_Init failed: testing init error msg",
      SDLInitFailedError);

  CHECK(true);
}

#else

TEST_CASE("testing that the SDL constructor sets an error when "
          "Init fails with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0))
      .Times(1)
      .WillOnce(testing::Return(-1));

  const char *testing_init_error_msg = "testing init error msg";
  EXPECT_CALL(*mock_sdl_wrapper, GetError())
      .Times(1)
      .WillOnce(testing::Return(testing_init_error_msg));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(0);

  SDLTester sdl_tester(mock_sdl_wrapper);

  if (sdl_tester.sdl) {
    CHECK_EQ(sdl_tester.sdl->valid(), false);
    CHECK_EQ(sdl_tester.sdl->get_last_error(),
             sdl_opengl_cpp::error::SDLInitFailedError);
  } else {
    CHECK(false);
  }

  CHECK(!sdl_tester.initialized());
  CHECK(true);
}

#endif
