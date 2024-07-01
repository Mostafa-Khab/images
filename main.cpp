#include <string>

#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img;
  gfx::image img2;
  

  img.load("/home/sasa/Projects/gfx-projects/games/bird/assets/bird64.png");
  img2.create(128, 128, 4);

  //img2.mask(0, 0, 0);

  img2.overlay(img, 50, 50);
  img2.save("test.png");
}

