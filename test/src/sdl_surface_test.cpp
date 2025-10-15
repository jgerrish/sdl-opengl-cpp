#include <doctest/doctest.h>
#include <memory>

#ifndef NO_EXCEPTIONS
#include "spdlog/spdlog.h"
#endif

#define HAVE_OPENGL

#include "SDL_opengl.h"
#include "mock_sdl.h"
#include "opengl.h"
#include "sdl_surface_test.h"

using ::testing::_;
using testing::Assign;
using testing::ByRef;
using testing::SaveArg;
using testing::SetArgPointee;
using testing::StrEq;

using namespace sdl_opengl_cpp;
using namespace sdl_opengl_cpp::sdl_surface;

SDLSurfaceTester::SDLSurfaceTester(const std::shared_ptr<SDL> &sdl_) {
  sdl_surface.emplace(
      SDLSurface(sdl_, 0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888));
}

SDL_Surface *SDLSurfaceTester::sdl_surface_handle() {
  return sdl_surface->surface;
}

void SDLSurfaceTester::set_invalid_sdl_surface() {
  sdl_surface->surface = nullptr;
}

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDLSurface constructor works with exceptions") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper, CreateRGBSurfaceWithFormat(
                                     0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888))
      .Times(1)
      // .Field(&MockSDL::initialized, testing::IsTrue())
      .WillOnce(testing::Return(reinterpret_cast<SDL_Surface *>(1)));

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  SDLSurfaceTester sdl_surface_tester(sdl);
  EXPECT_CALL(*mock_sdl_wrapper,
              FreeSurface(sdl_surface_tester.sdl_surface_handle()))
      .Times(1);

  CHECK(true);
}

#else

TEST_CASE(
    "testing that the SDLSurface constructor works with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper, CreateRGBSurfaceWithFormat(
                                     0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888))
      .Times(1)
      // .Field(&MockSDL::initialized, testing::IsTrue())
      .WillOnce(testing::Return(reinterpret_cast<SDL_Surface *>(1)));

  SDLSurfaceTester sdl_surface_tester(sdl);

  EXPECT_CALL(*mock_sdl_wrapper,
              FreeSurface(sdl_surface_tester.sdl_surface_handle()))
      .Times(1);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  CHECK(true);
}

#endif

#ifndef NO_EXCEPTIONS

TEST_CASE("testing that the SDLSurface constructor throws an exception when "
          "CreateRGBSurfaceWithFormat fails with exceptions enabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper, CreateRGBSurfaceWithFormat(
                                     0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888))
      .Times(1)
      // .Field(&MockSDL::initialized, testing::IsTrue())
      .WillOnce(testing::Return(reinterpret_cast<SDL_Surface *>(0)));

  EXPECT_CALL(*mock_sdl_wrapper, FreeSurface(_)).Times(0);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  CHECK_THROWS_WITH_AS([sdl] { SDLSurfaceTester sdl_surface_tester(sdl); }(),
                       "ERROR::SDL_SURFACE::CREATE_SDL_SURFACE_FAILED",
                       sdl_surface::CreationError);

  CHECK(true);
}

#else

TEST_CASE("testing that the SDLSurface constructor sets an error when "
          "CreateSurface fails with exceptions disabled") {
  GL_Context gl_context = {};

  std::shared_ptr<GL_Context> glcontext =
      std::make_shared<GL_Context>(gl_context);

  std::shared_ptr<MockSDLWrapper> mock_sdl_wrapper =
      std::make_shared<MockSDLWrapper>();

  EXPECT_CALL(*mock_sdl_wrapper, Init(0)).Times(1).WillOnce(testing::Return(0));

  std::shared_ptr<SDL> sdl = std::make_shared<SDL>(mock_sdl_wrapper);

  EXPECT_CALL(*mock_sdl_wrapper, CreateRGBSurfaceWithFormat(
                                     0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888))
      .Times(1)
      // .Field(&MockSDL::initialized, testing::IsTrue())
      .WillOnce(testing::Return(reinterpret_cast<SDL_Surface *>(0)));

  SDLSurfaceTester sdl_surface_tester(sdl);

  // Doesn't get called because SDL_Surface pointer is null
  EXPECT_CALL(*mock_sdl_wrapper,
              FreeSurface(sdl_surface_tester.sdl_surface_handle()))
      .Times(0);

  EXPECT_CALL(*mock_sdl_wrapper, Quit()).Times(1);

  if (sdl_surface_tester.sdl_surface) {
    CHECK_EQ(sdl_surface_tester.sdl_surface->valid(), false);
    CHECK_EQ(sdl_surface_tester.sdl_surface->get_last_error(),
             sdl_opengl_cpp::error::SDLSurfaceCreationError);
  } else {
    CHECK(false);
  }

  CHECK_EQ(sdl_surface_tester.sdl_surface_handle(), nullptr);

  CHECK(true);
}

#endif
