#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img;
  gfx::image img2;

  img = gfx::image("/home/sasa/Projects/gfx-projects/games/bird/assets/bird64.png");

  img2.create(64, 64, 4, img.data);

  img2.save("test.png");
}

