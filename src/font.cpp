#include <logger.hpp>

#include "font.hpp"

namespace gfx
{
  FT_Library font::library;
  bool font::has_freetype = false;

  font::font(): m_done(false)
  {
    if(has_freetype) 
      return;

    int error = FT_Init_FreeType(&library);
    if(error)
    {
      Log::error("failed to init freetype");
      Log::error("unable to construct font object");
      return;
    }

    has_freetype = true;
  }

  font::~font()
  {
    if(!has_freetype)
      return;

    if(!m_done)
      FT_Done_Face(face);

    FT_Done_FreeType(library);
  }

  void font::set_size(int w, int h)
  {
    FT_Set_Pixel_Sizes(face, w, h);
  }

  bool font::load(std::string file)
  {
    int error = FT_New_Face(library, file.c_str(), 0, &face);
    if(error)
      Log::error("failed to load font file at: " + file);

    return !error;
  }

  void font::done()
  {
    if(m_done)
      return;

    FT_Done_Face(face);
    m_done = true;
  }

  font::character font::get(int code)
  {
    int error = FT_Load_Char(face, code, FT_LOAD_RENDER);
    if(error)
      Log::error("failed to load requested character with code: " + std::to_string(code));

    return character{
      (int)face->glyph->bitmap.width,
      (int)face->glyph->bitmap.rows,
      (int)(face->glyph->advance.x >> 6),
      face->glyph->bitmap.buffer };
  }

}
