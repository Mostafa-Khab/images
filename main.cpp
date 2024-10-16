#include <iostream>
#include <string>
#include <memory>

#include "src/image.hpp"
#include "src/font.hpp"

int main(int argc, const char* argv[])
{

  gfx::kernel_t kernel =
  {
    {1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f},
  };

  gfx::image img;
  bool result = img.load("../assets/bird64.png");

  if(!result)
    img.load("bird64.png");

  if(!result)
    return 1;


  img.apply_kernel(kernel, kernel.size() * kernel.size());

  img.save("test2.png");

  return 0;
}

