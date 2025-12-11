# This is the actual CMake project script for the install integration
# test.
#
# Unlike the main project and build integration test, this is required
# because we test both find_package and ExternalProject.
# ExternalProject won't make the *.cmake files available until a
# target is explicitly built.
cmake_minimum_required(VERSION 3.24)

project(sdl-opengl-cpp-install-integration-test VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Follow the current Google C++ Style Guides and target C++ 20
# They are aggressive in their adoption of C++ versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  ${PROJECT_NAME}
  src/${PROJECT_NAME}.cpp
)

find_package(SDL2 CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 CONFIG REQUIRED SDL2main)
find_package(spdlog REQUIRED)

# Take care of the other dependencies first.
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

if(spdlog_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()

message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

message("installDir: ${installDir}")

# This isn't needed when using OVERRIDE_FIND_PACKAGE
list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

# find_package(sdl-opengl-cpp)
find_package(sdl-opengl-cpp CONFIG REQUIRED)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_INSTALL_PREFIX}/include/sdl-opengl-cpp
)
# target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_INSTALL_PREFIX}/lib/libsdl-opengl-cpp.a)

target_link_libraries(${PROJECT_NAME} PUBLIC sdl-opengl-cpp::sdl-opengl-cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
  COMPONENT runtime
)
