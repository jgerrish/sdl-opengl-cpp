cmake_minimum_required(VERSION 3.13)

# This is a C++ library of simple RAII classes to manage OpenGL
# structures and functions.
# Right now, that includes programs, shaders, Vertex Array Objects and
# Vertex Buffer Objects.
project(sdl-opengl-cpp VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Follow the current Google C++ Style Guides and target C++ 20
# They are aggressive in their adoption of C++ versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Turn warnings into errors
# set(CMAKE_REQUIRED_FLAGS "-fvisibility=hidden -Werror -Wall -Wextra -Wshadow")

# Compiler info

if(CMAKE_C_COMPILER_ID MATCHES "Clang|IntelLLVM")
  set(USE_CLANG TRUE)
  set(OPT_DEF_ASM TRUE)
  # Visual Studio 2019 v16.2 added support for Clang/LLVM.
  # Check if a Visual Studio project is being generated with the Clang toolset.
  if(MSVC)
    set(MSVC_CLANG TRUE)
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(USE_GCC TRUE)
  set(OPT_DEF_ASM TRUE)
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(USE_GNUCXX TRUE)
  set(OPT_DEF_ASM TRUE)
elseif(MSVC_VERSION GREATER 1400) # VisualStudio 8.0+
  set(OPT_DEF_ASM TRUE)
  #set(CMAKE_C_FLAGS "/ZI /WX- /
elseif(CMAKE_C_COMPILER_ID MATCHES "^Intel$")
  set(OPT_DEF_ASM TRUE)
  set(USE_INTELCC TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "QCC")
  set(USE_QCC TRUE)
else()
  set(OPT_DEF_ASM FALSE)
endif()

if(USE_GCC OR USE_CLANG OR USE_INTELCC OR USE_QCC)
  set(OPT_DEF_GCC_ATOMICS ON)
endif()

# A lot of this is duplicated in scripts/cmake/common.cmake
if(USE_GCC OR USE_CLANG OR USE_INTELCC)
  include(CheckCompilerFlag)
  if(HAVE_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wshadow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wshadow")
    set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -Werror -Wall -Wextra -Wshadow")
  endif()
  # cmake_pop_check_state()
elseif(USE_GNUCXX)
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-Werror HAVE_WERROR)
  if(HAVE_WERROR)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wshadow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wshadow")
    set(CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -Werror -Wall -Wextra -Wshadow")
  endif()
endif()


# This will be configurable
# I think we'll want to build two test targets,
# _with_exceptions and _without_exceptions
# The CI system can then run both of them.

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_EXCEPTIONS=1")

include_directories(include)

add_library(
  ${PROJECT_NAME}
  src/gl_context.cpp
  src/vertex_buffer_object.cpp
  src/vertex_array_object.cpp
  src/shader.cpp
  src/program.cpp
)

add_subdirectory(src)

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

# Setup documentation with Doxygen
# This is copied from the GLFW3 project
# https://github.com/glfw/glfw
# The license at the time it was copied is included below.
#
# Copyright (c) 2002-2006 Marcus Geelnard
#
# Copyright (c) 2006-2019 Camilla Löwy
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:

# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would
#    be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such, and must not
#    be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source
#    distribution.
find_package(Doxygen)

if(Doxygen_FOUND)
  message("Found Doxygen")
  doxygen_add_docs(
    doxygen
    ${PROJECT_SOURCE_DIR}
    COMMENT "Generate documentation"
  )
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.doxygen
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  # set_target_properties(docs PROPERTIES FOLDER "GLFW3")
endif()


# Setup logging with spdlog

find_package(spdlog REQUIRED)

if(spdlog_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()


add_subdirectory(test)
