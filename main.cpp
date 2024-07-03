#include <iostream>
#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

int main(int argc, const char* argv[])
{
  float kernal[3][3] = 
  {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1} 
  };

  gfx::image img;
  img.load("/home/sasa/Pictures/bird64.png");
  img.apply_kernal(kernal);

  img.save("test.png");

}

