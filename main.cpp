#include <string>
#include <iostream>

#include "src/image.hpp"
#include "src/font.hpp"

const int font_size = 256;

int main(int argc, const char* argv[])
{
  std::wstring str(L"pause game");

  gfx::font font;
  font.load("/usr/local/share/fonts/HurmitNerdFont/HurmitNerdFont-Regular.otf");
  font.set_size(0, font_size);

  gfx::image img;
  img.create(str, font);
  img.save("test.jpg");

}

