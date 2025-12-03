# build tree integration test
#
# This tests pulling the project in with ExternalProject and using it
include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.24)

project(sdl-opengl-cpp-build-tree-integration-test VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Follow the current Google C++ Style Guides and target C++ 20
# They are aggressive in their adoption of C++ versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  ${PROJECT_NAME}
  src/${PROJECT_NAME}.cpp
)


find_package(OpenGL REQUIRED)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 CONFIG REQUIRED SDL2main)

if (SDL2_FOUND)
 	message("Using SDL2 from find_package")
 	target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
 	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
 	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
 	unset(CHECK_SDL_VERSION CACHE)
else()
 	# Fall back on pkg-config
 	include(FindPkgConfig)
 	pkg_search_modules(SDL2 REQUIRED sdl2)
 	message("Using SDL2 from pkg-config")
	target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
 	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
 	set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIRS})
 	unset(CHECK_SDL_VERSION CACHE)
endif()

find_package(spdlog REQUIRED)

if(spdlog_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()

# This integration test only uses ExternalProject to build and install
# sdl-opengl-cpp into a local install folder and link against the
# built code.
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
)

add_dependencies(${PROJECT_NAME} "sdl-opengl-cpp-install")

ExternalProject_Get_Property("sdl-opengl-cpp" INSTALL_DIR)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${INSTALL_DIR}/include/sdl-opengl-cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${INSTALL_DIR}/lib/libsdl-opengl-cpp.a)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)
