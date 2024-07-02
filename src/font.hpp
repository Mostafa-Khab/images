/*********************************
 * author     : mostafa khaled
 * date       : Tue Jul  2 09:32:20 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef FONT_HPP
#define FONT_HPP

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H


namespace gfx
{
  class font
  {
    public:
      struct character;
    public:
      font();
     ~font();

      void set_size(int w, int h);
      bool load(std::string path);
      void done();

      character get(int code);
    public:
      FT_Face              face;
      static FT_Library library;
    private:
      static bool has_freetype;
      bool m_done;
  };

  struct font::character
  {
    int w, h, advance;
    std::uint8_t* data;
  };
}

#endif /* !FONT_HPP */
