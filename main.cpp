#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

const int font_size = 48;

int main(int argc, const char* argv[])
{
  std::wstring str(L" hello world");
  gfx::font font;
  font.load("/usr/local/share/fonts/HackNerdFont/HackNerdFont-Regular.ttf");
  font.set_size(0, font_size);

  gfx::image img;
  img.create(str, font);

  gfx::image text;
  text.create(img.width, img.height, 4);
  text.mask(0, 0, 0);

  for(int i = 0; i < text.height; ++i)
  {
    for(int j = 0; j < text.width; ++j)
    {
        text.data[(i * text.width + j) * text.channels + 3] = img.data[(i * img.width + j)];
    }
  }

  text.save("test.png");
}

