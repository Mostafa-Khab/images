/*********************************
 * author     : mostafa khaled
 * date       : Thu Oct 17 07:51:34 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct Font_char
{
  unsigned int w, h, advance;
  int bx, by;                   //bearing x, bearing y
  unsigned char* data;
} Font_char; 

void Font_init();
void Font_load(const char* filepath);
void Font_setSize(unsigned int h);
void Font_getc(Font_char* fc, unsigned long code);
void Font_terminate();

#endif /* !FONT_H */
