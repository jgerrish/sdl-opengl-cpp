#include "sdl_base.h"

int main(int argc, char **argv)
{
  // Just a basic creation of the SDL object, we don't need to do
  // anything really.
  std::shared_ptr<sdl_opengl_cpp::SDL> sdl = std::make_shared<sdl_opengl_cpp::SDL>();

  return 0;
}
