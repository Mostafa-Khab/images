#include <cstring>
#include "src/image.hpp"

int main(int argc, const char* argv[])
{
  gfx::image img;

  img = gfx::image("/home/sasa/Pictures/gruv.jpg");

  for(long i = 0; i < img.size(); i += img.channels)
  {
    int sum = 0;
    for(int c = 0; c < img.channels; ++c)
    {
      sum += img.data[i + c];
    }

    std::memset(&img.data[i], sum / 3.f, img.channels);
  }

  img.save("test.jpg");
}

