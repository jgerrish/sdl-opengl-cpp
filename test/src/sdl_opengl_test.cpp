#include <doctest/doctest.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#include "gl_context.h"
#include "mock_opengl.h"
#include "mock_sdl.h"
#include "sdl_opengl_runner.h"
#include "sdl_opengl_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_opengl;

SDLOpenGLTester::SDLOpenGLTester(const std::shared_ptr<SDL> &sdl,
                                 const std::shared_ptr<GLContext> &ctx) {
  sdl_opengl.emplace(sdl_opengl_cpp::SDLOpenGL(sdl, ctx));
}

// #ifndef NO_EXCEPTIONS

// TEST_CASE("testing that SDLOpenGL constructor works with exceptions") {
//   spdlog::info("testing that SDLOpenGL constructor works with exceptions");
//   GL_Context gl_context = {};

//   std::shared_ptr<GL_Context> glcontext =
//       std::make_shared<GL_Context>(gl_context);

//   std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
//       std::make_shared<MockSDLWrapper>(SDL_INIT_VIDEO);

//   EXPECT_CALL(*mock_sdl_wrapper,
//   Init(0)).Times(1).WillOnce(testing::Return(0));

//   std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

//   std::shared_ptr<MockOpenGLContext> mock_opengl_context =
//       std::make_shared<MockOpenGLContext>(glcontext);

//   EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

//   SDLOpenGLTester sdl_opengl_tester(sdl, mock_opengl_context);

//   CHECK(true);
// }

// #else

// TEST_CASE("testing that SDLOpenGL constructor works with exceptions
// disabled") {
//   GL_Context gl_context = {};

//   std::shared_ptr<GL_Context> glcontext =
//       std::make_shared<GL_Context>(gl_context);

//   std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
//       std::make_shared<MockSDLWrapper>();

//   EXPECT_CALL(*mock_sdl_wrapper,
//   Init(0)).Times(1).WillOnce(testing::Return(0));

//   std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

//   std::shared_ptr<MockOpenGLContext> mock_opengl_context =
//       std::make_shared<MockOpenGLContext>(glcontext);

//   EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

//   SDLOpenGLTester sdl_opengl_tester(sdl, mock_opengl_context);

//   CHECK(true);
// }
// #endif
