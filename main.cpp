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
  img.save("test.jpg");
}

