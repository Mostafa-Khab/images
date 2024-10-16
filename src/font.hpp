/*********************************
 * author     : mostafa khaled
 * date       : Tue Jul  2 09:32:20 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef FONT_HPP
#define FONT_HPP

#include <string>

struct FT_LibraryRec_;
struct FT_FaceRec_;

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
      //FT_Face but the origin not typedef
      FT_FaceRec_*           face;          

      //FT_Library but the origin not typedef
      static FT_LibraryRec_*& library() { static FT_LibraryRec_* ftlib; return ftlib; };

      //FT font size.
      static int&             size() { static int size; return size;};
    private:
      static bool has_freetype;
      bool m_done;
      bool m_loaded;

      friend class image;
  };

  struct font::character
  {
    int w, h, advance;
    int bearing_x, bearing_y;
    std::uint8_t* data;
  };
}

#endif /* !FONT_HPP */
