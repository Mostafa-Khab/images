#include <iostream>
#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

int main(int argc, const char* argv[])
{

  std::vector<std::vector<float>> kernel = 
  {
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
  };

  gfx::image img;
  img.load("/home/sasa/Pictures/bird64.png");

  img.apply_kernel(kernel, kernel.size() * kernel.size());
  img.save("test2.png");
}

