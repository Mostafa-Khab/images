#include <iostream>
#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

int main(int argc, const char* argv[])
{
  float kernal[3][3] = 
  {
    {1.f, 1.f, 1.f},
    {1.f, 0.f, 1.f},
    {1.f, 1.f, 1.f} 
  };

  gfx::image img;
  img.load("/home/sasa/Pictures/test.jpg");

  img.apply_kernal(kernal);
  img.apply_kernal(kernal);
  img.apply_kernal(kernal);

  img.save("test.jpg");

}

