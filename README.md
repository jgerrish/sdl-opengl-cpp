# sdl-opengl-wrapper: A C++ wrapper for OpenGL under SDL

# Introduction #

This is a C++ wrapper for working with OpenGL under SDL.

It provides classes for vertex buffer objects, vertex array objects,
shaders and programs.

# Installation #

## Prerequisites ##

The following build tools are required:
CMake

The following libraries are required:
SDL2
spdlog
gMock (sometimes included with your distro's gTest package)
doctest
Doxygen
pkg-config

On Ubuntu, they can be installed with the following:

apt install cmake doctest-dev doxygen google-mock googletest libsdl2-dev libspdlog-dev pkg-config

doctest can be downloaded from GitHub:
[doctest](https://github.com/doctest/doctest.git)


You also need the basic C++ toolchain.  It's the build-essential
package on Debian systems, Fedora and SUSE both have meta-packages or
groups that you can install to get C++ development.

The examples below use the Ninja build system:

apt install ninja-build

## Compiling ##

$ mkdir build

For debug builds:

$ cmake -DCMAKE\_BUILD\_TYPE=Debug ..

For release builds:

$ cmake -DCMAKE\_BUILD\_TYPE=Debug ..

To build it:

$ cd build
$ make


You can test the library with the following command:

$ test/sdl-opengl-cpp-test



### Windows ###

To compile for Windows systems.

This project has been tested in Visual Studio 2022 with the Desktop
development with C++ Workload installed.

You'll also need a copy of CMake installed and vcpkg.


Install prerequisites with vcpkg:

For 64-bit machines:

vcpkg.exe install doctest gtest[cxx17] sdl2 spdlog --triplet x64-windows


Generate CMake configuration

Open cmake-gui up.

Select the sdl-opengl-cpp directory as the source directory in the
"Where's the source code" edit box.

Select the sdl-opengl-cpp directory as the build directory in the
"Where to build the binaries" edit box.  Then append build to the
path.  So the path should be something like:

BLAHBLAHBLAH/sdl-opengl-cpp/build

Click Configure.

Choose the option Specify toolchain file for cross-compiling and click
Next.

Select vcpkg.cmake in the scripts/buildsystems directory of your vcpkg
install.

Click Finish.

Click Generate after it is done configuring.

Click Open Project after it is done generating.


In Visual Studio

Right click on the sdl-opengl-cpp-test project and click build.
Execute it from a command prompt to test it.


# Testing #

To run the tests, just execute the following after compiling:

$ test/sdl-opengl-cpp-test

## Integration Tests ##

There are now a set of separate integration tests that test
integrating sdl-opengl-cpp into another project.  There are currently
two separate tests.

One tests compiling from a build-tree download (using CMake's
ExternalProject_Add).

The other tests compiling using the installed
sdl-opengl-cpp-config.cmake files.

These tests can be built separately without building the main project
first.  They setup their own build directories and try to simulate as
much as possible the process of another project using sdl-opengl-cpp.

### Build-tree Integration Test ##

To build and run the build-tree integration test:

$ cd test/integration/build-tree-integration-test
$ mkdir -p build
$ cd build
$ cmake -DCMAKE\_BUILD\_TYPE=Debug -G Ninja ..
$ ninja
$ ./sdl-opengl-cpp-build-tree-integration-test

The process return code should be 0.  To test under bash:

$ echo $?

### Install Integration Test ##

To build and run the install integration test:

$ cd test/integration/install-integration-test
$ mkdir -p build
$ cd build
$ cmake e -DCMAKE\_BUILD\_TYPE=Debug -G Ninja ..
$ ninja
$ ./install/bin/sdl-opengl-cpp-install-integration-test

The process return code should be 0.  To test under bash:

$ echo $?
