#include <string>
#include <iostream>

#include "src/image.hpp"
#include "src/font.hpp"

const int font_size = 128;

int main(int argc, const char* argv[])
{
  std::wstring str;

  for(int i = 61502; i <= 61522; ++i)
  {
    str += (wchar_t)i;
    str += ' ';
  }
  
  gfx::font font;
  font.load("/usr/local/share/fonts/HackNerdFont/HackNerdFont-Bold.ttf");
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

  std::cout << (int)L'' << '\n';
  text.save("test.png");
}

