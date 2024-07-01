#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img;
  gfx::image img2;

  img = gfx::image("/home/sasa/Pictures/gruv.jpg");
  img2 = img.crop(64,0, 128, 64);

  img2.save("test.png");
}

