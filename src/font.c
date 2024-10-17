#include <stdlib.h>

#include "log.h"
#include "font.h"

#define CHECK(X, Y) \
  if((X)) {         \
    log(Y);         \
    exit(-1);       \
  }

FT_Library   library;
FT_Face      face;
unsigned int Font_size;

void Font_init()
{
  int e = FT_Init_FreeType(&library);
  CHECK(e, "failed to init freetype");
}

void Font_load(const char* filepath)
{
  int e = FT_New_Face(library, filepath, 0, &face);
  CHECK(e, "failed to create freetype font face");
}

void Font_setSize(unsigned int h)
{
  Font_size = h;
  FT_Set_Pixel_Sizes(face, 0, h);
}

void Font_getc(Font_char* fc, unsigned long code)
{
  int e = FT_Load_Char(face, code, FT_LOAD_RENDER | FT_LOAD_COLOR);
  CHECK(e, "failed to load requested char");

  fc->w = face->glyph->bitmap.width;
  fc->h = face->glyph->bitmap.rows;
  fc->advance = face->glyph->advance.x >> 6;
  fc->bx      = face->glyph->bitmap_left;
  fc->by      = face->glyph->bitmap_top;
  fc->data    = face->glyph->bitmap.buffer;

  assert(face->glyph->bitmap.buffer);
}

void Font_terminate()
{
  FT_Done_Face(face);
  FT_Done_FreeType(library);
}
