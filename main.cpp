#include <string>

#include "src/image.hpp"
#include "src/font.hpp"

const int font_size = 48;

int main(int argc, const char* argv[])
{
  std::string str("hello");
  gfx::font font;
  font.load("/usr/share/fonts/truetype/hack/Hack-Regular.ttf");
  font.set_size(0, font_size);

  gfx::image atlas;
  atlas.create(str.size() * font_size, font_size, 1);

  int pen = 0;
  for(int i = 0; i < str.size(); ++i)
  {
    auto ch = font.get(str[i]);
    gfx::image c;
    c.create(ch.w, ch.h, 1, ch.data);
    atlas.overlay(c, pen , 0);
    pen += ch.advance;
  }

  atlas.save("test.jpg");
}

