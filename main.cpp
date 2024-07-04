#include <iostream>
#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

int main(int argc, const char* argv[])
{
  float kernal[5][5] = 
  {
    {1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f},
    {1.f, 1.f, 1.f, 1.f, 1.f},
  };

  float circle[5][5] = 
  {
    {0.f, 0.f, 1.f, 0.f, 0.f},
    {0.f, 1.f, 1.f, 1.f, 0.f},
    {1.f, 1.f, 1.f, 1.f, 1.f},
    {0.f, 1.f, 1.f, 1.f, 0.f},
    {0.f, 0.f, 1.f, 0.f, 0.f},
  };

  gfx::image img;
  img.load("/home/sasa/Pictures/bird64.png");

  img.apply_kernal(circle, 13);

  img.save("test.png");
}

