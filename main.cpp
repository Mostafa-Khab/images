#include <string>
#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img;

  img.create(64, 64, 4);
  img.mask(1, 0.45, 0.15);
  img.save("test.png");
}

