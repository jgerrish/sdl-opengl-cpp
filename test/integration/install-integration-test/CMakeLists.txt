# sdl-opengl-cpp-config.cmake integration tests
#
# This tests installs of sdl-opengl-cpp, in particular using
# find_package and the sdl-opengl-cpp-*.cmake files
#
# We had to use two separate ExternalProject calls here, because
# ExternalProject doesn't make the targets available like FetchContent
# does.
#
# It uses what's called a "super-build" pattern in CMake, which is
# common in larger projects.
#
# The actual project itself is in a subdirectory and pulled in via an
# ExternalProject_Add call.
cmake_minimum_required(VERSION 3.24)

project(sdl-opengl-cpp-install-integration-test-super-build VERSION 0.1.0)

include(CMakePrintHelpers)

find_package(OpenGL REQUIRED)
# target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

# find_package(fmt REQUIRED)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 CONFIG REQUIRED SDL2main)
find_package(spdlog REQUIRED)

# First, we use ExternalProject to build and install sdl-opengl-cpp
# into the build tree of this integration test.
#
# We don't want to build the sdl-opengl-cpp tests.
#
# That's part of the reason we're using ExternalProject instead of
# FetchContent.  We have a little more control with it.
#
# This first commit doesn't disable test building in the entire
# project.  I need to make sure that all the CI scripts properly run
# the tests.  And it's also a social design decision with project
# management.  Running tests by default may be a better choice for
# future new project contributors.
set(installDir ${CMAKE_CURRENT_BINARY_DIR}/install)

include(ExternalProject)

ExternalProject_Add(
  "sdl-opengl-cpp"
  DEPENDS OpenGL::GL SDL2::SDL2 spdlog::spdlog
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../
  INSTALL_DIR ${installDir}

  # We don't want to build the sdl-opengl-cpp tests.
  CMAKE_ARGS -DBUILD_TESTING:BOOL=NO
             -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
             -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>

  # Disable installs
  # INSTALL_COMMAND ""

  BUILD_BYPRODUCTS ${SDL_OPENGL_CPP_INCLUDE_DIR} ${SDL_OPENGL_CPP_LIB} ${installDir}/lib/libsdl-opengl-cpp.a

  STEP_TARGETS build install

  # This is one way to integrate find_package and ExternalProjet
  #
  # But it's not as close to real-world as using the install
  # directory alone.
  # OVERRIDE_FIND_PACKAGE
)

ExternalProject_Add(
  "sdl-opengl-cpp-install-integration-test"
  DEPENDS "sdl-opengl-cpp-install"
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install-integration-test/
  INSTALL_DIR ${installDir}

  # We don't want to build the sdl-opengl-cpp tests.
  CMAKE_ARGS -DBUILD_TESTING:BOOL=NO
             -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
             -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>

  STEP_TARGETS build install

  # Disable installs
  # INSTALL_COMMAND ""
)
