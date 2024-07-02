#include <iostream>
#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img("/home/sasa/Projects/gfx-projects/games/bird/assets/bird64.png");


  img.flip_channel_y(0);
  img.save("test.jpg");
}

