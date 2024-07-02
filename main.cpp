#include <iostream>
#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img("/home/sasa/Projects/gfx-projects/games/bird/assets/bird64.png");


  img.crop(64, 0, 64, 64);
  img.save("test.jpg");
}

