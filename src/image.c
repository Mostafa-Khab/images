#include <malloc.h>
#include <string.h>
#include <wchar.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "log.h"
#include "image.h"
#include "font.h"

#define IMAGE_HEADER_SIZE 64

void Image_alloc(Image* img, unsigned int w, unsigned int h, unsigned int c) {
#ifdef PRINT_LOG
  printf("INFO: allocating %ux%u Image with %u components\n", w, h, c);
#endif

  img->data = (unsigned char*)malloc(w * h * c * sizeof(unsigned int));
  ASSERT(img->data != NULL);
  img->w = w;
  img->h = h;
  img->c = c;
}

void Image_load(Image* img, const char* filepath) {
  int w, h, c;
  img->data = (unsigned char*)stbi_load(filepath, &w, &h, &c, 0);
  ASSERT(img->data != NULL);

  img->w = w;
  img->h = h;
  img->c = c;
}

void Image_save(Image img, const char* filepath) {
    ASSERT(img.data != NULL);

    const char* ext = &filepath[strlen(filepath) - 3];
    if(strcmp(ext, "png") == 0) {
       stbi_write_png(filepath, img.w, img.h, img.c, img.data, img.w * img.c);
    } else if(strcmp(ext, "jpg") == 0 || strcmp(&filepath[strlen(filepath) - 4], "jpeg") == 0) {
       stbi_write_jpg(filepath, img.w, img.h, img.c, img.data, 90);
    } else if(strcmp(ext, "tga")) {
       stbi_write_tga(filepath, img.w, img.h, img.c, img.data);
    } else if(strcmp(ext, "bmp")) {
       stbi_write_bmp(filepath, img.w, img.h, img.c, img.data);
    } else {
       LOG("No valid image format");
    }
}

void Image_fill(Image* img, uint8_t color) {
  memset(img->data, color, img->w * img->h * img->c);
}

void Image_free(Image* img)
{
  free(img->data);
  img->data = NULL;
  img->w = 0;
  img->h = 0;
  img->c = 0;
}

void Image_mask(Image img, float* mask) {
  ASSERT(img.data != NULL);

  for(unsigned int i = 0; i < img.h; ++i) 
    for(unsigned int j = 0; j < img.w; ++j) 
      for(unsigned int c = 0; c < img.c; ++c) 
        img.data[(i * img.w + j) * img.c + c] = mask[c] * img.data[(i * img.w + j) * img.c + c];
}

void Image_overlay(Image bg, Image fg, int x, int y) {
  ASSERT(bg.data != NULL);
  ASSERT(fg.data != NULL);

  for(unsigned int i = 0; i < fg.h; ++i)
  {
    if(i + y < 0) continue;
    if(i + y >= bg.h) break;

    for(unsigned int j = 0; j < fg.w; ++j)
    {
      if(j + x >= bg.w) break;
      if(j + x < 0) continue;

      uint8_t* d = &bg.data[((i + y) * bg.w + (j + x)) * bg.c];
      uint8_t* s = &fg.data[(i * fg.w + j) * fg.c];

      float s_alpha = (fg.c < 4)? 1 : s[3] / 255.f;
      //float d_alpha = (    channels < 4)? 1 : d[3] / 255.f;

      for(unsigned int c = 0; c < fg.c && c < bg.c; ++c)
      {
        d[c] = s[c] * s_alpha + d[c] * (1.f - s_alpha);
      }
    }
  }
}

void Image_crop(Image* img, unsigned int  x, unsigned int  y, unsigned int  w, unsigned int  h)
{
  ASSERT(img->data != NULL);
  ASSERT(x >= 0 && y >= 0);

  Image cropped;
  Image_alloc(&cropped,w ,h,img->c);

  for(int i = 0; i < h; ++i)
  {
    if(i + y >= img->h) break;
    for(int j = 0; j < w; ++j)
    {
      if(j + x >= img->w) break;

      memcpy(
          &cropped.data[(i * w + j) * img->c],
          &img->data[((i + y) * img->w + (j + x)) * img->c],
          img->c
       );
    }
  }

  Image_free(img);
  *img = cropped;
}

void Image_grayscale_avg(Image img) 
{
  ASSERT(img.data != NULL);

  if(img.c < 3)
    LOG("this image is already in grayscale");

  for(int i = 0; i < img.w * img.h * img.c; i += img.c)
  {
    int gray = (img.data[i] + img.data[i + 1] + img.data[i + 2]) / 3;
    memset(&img.data[i], gray, 3);
  }
}

void Image_grayscale_lum(Image img) 
{
  ASSERT(img.data != NULL);

  if(img.c < 3)
    LOG("this image is already in grayscale");

  for(int i = 0; i < img.w * img.h * img.c; i += img.c)
  {
    int gray = (0.2126f * img.data[i] + 0.7152 * img.data[i + 1] + 0.0722 * img.data[i + 2]) / 3;
    memset(&img.data[i], gray, 3);
  }
}

void Image_flip_x(Image img) 
{
  ASSERT(img.data != NULL);
  uint8_t temp[4] = {0};
  for(unsigned int i = 0; i < img.h; ++i) {
    for(unsigned int j = 0; j < img.w / 2; ++j) {
       uint8_t* l = &img.data[(i * img.w + j) * img.c];
       uint8_t* r = &img.data[(i * img.w + (img.w - j - 1)) * img.c];

       memcpy(temp, l, img.c);
       memcpy(l, r, img.c);
       memcpy(r, temp, img.c);
    }
  }
}

void Image_flip_y(Image img) 
{
  ASSERT(img.data != NULL);
  uint8_t temp[4] = {0};
  for(unsigned int i = 0; i < img.h / 2; ++i) {
    for(unsigned int j = 0; j < img.w; ++j) {
       uint8_t* u = &img.data[(i * img.w + j) * img.c];
       uint8_t* d = &img.data[((img.h - 1 - i) * img.w + j) * img.c];

       memcpy(temp, u, img.c);
       memcpy(u, d, img.c);
       memcpy(d, temp, img.c);
    }
  }
}

void Image_flip_channel_x(Image img, uint8_t channel) 
{
  ASSERT(img.data != NULL);
  for(unsigned int i = 0; i < img.h; ++i) {
    for(unsigned int j = 0; j < img.w / 2; ++j) {
       uint8_t temp = img.data[(i * img.w + j) * img.c + channel];
       img.data[(i * img.w + j) * img.c + channel] = img.data[(i * img.w + (img.w - j - 1)) * img.c + channel];
       img.data[(i * img.w + (img.w - j - 1)) * img.c + channel] = temp;
    }
  }
}

void Image_flip_channel_y(Image img, uint8_t channel) 
{
  ASSERT(img.data != NULL);
  for(unsigned int i = 0; i < img.h / 2; ++i) {
    for(unsigned int j = 0; j < img.w; ++j) {
       uint8_t temp = img.data[(i * img.w + j) * img.c];
       img.data[(i * img.w + j) * img.c] = img.data[((img.h - 1 - i) * img.w + j) * img.c];
       img.data[((img.h - 1 - i) * img.w + j) * img.c] = temp;
    }
  }
}

void Image_embed(Image bg, const Image fg, const int x, const int y, uint8_t layer) 
{
  const unsigned int minc = bg.c > fg.c? fg.c : bg.c;
  for(int i = 0; i < fg.h; ++i)
  {
    if(i + y < 0) continue;
    if(i + y >= bg.h) break;

    for(int j = 0; j < fg.w; ++j)
    {
      if(j + x >= bg.w) break;
      if(j + x < 0) continue;

      uint8_t* d = &bg.data[((i + y) * bg.w + (j + x)) * bg.c];
      uint8_t* s = &fg.data[(i * fg.w + j) * fg.c];

      for(int c = 0; c < minc; ++c)
      {
        d[c] &= 0xfe;
        d[c] |= (s[c] >> layer) & 1;
      }
    }
  }
}

//extract the image layer from fg to bg
void Image_extract(Image bg, const Image fg, const int x, const int y, uint8_t layer) 
{
  for(int i = 0; i < fg.h; ++i)
  {
    if(i + y < 0) continue;
    if(i + y >= bg.h) break;

    for(int j = 0; j < fg.w; ++j)
    {
      if(j + x >= bg.w) break;
      if(j + x < 0) continue;

      uint8_t* d = &bg.data[((i + y) * bg.w + (j + x)) * bg.c];
      uint8_t* s = &fg.data[(i * fg.w + j) * fg.c];

      for(int c = 0; c < fg.c; ++c)
      {
        d[c] = (s[c] & 1) << layer;
      }
    }
  }
}

void Image_embed_text(Image dst, const char* text, const unsigned int offset) 
{
  ASSERT(strlen(text) * 8 + IMAGE_HEADER_SIZE <= dst.w * dst.h * dst.c - offset);
  ASSERT(dst.data != NULL);

  size_t len = strlen(text);
#ifdef PRINT_LOG
  printf("INFO: embeding text of size %lu\n", len);
#endif

  for(unsigned int  i = 0; i <= IMAGE_HEADER_SIZE; ++i) {
    dst.data[i + offset] &= 0xfe;
    dst.data[i + offset] |= ((len >> i) & 1);
  }

  for(unsigned int i = 0; i < len; ++i) {
    for(unsigned int j = 0; j < 8; ++j) {
      dst.data[i * 8 + IMAGE_HEADER_SIZE + j + offset] &= 0xfe;
      dst.data[i * 8 + IMAGE_HEADER_SIZE + j + offset] |= ((text[i] >> j) & 1);
    }
  }
}

void Image_extract_text(Image src, unsigned int  offset) 
{
    ASSERT(src.data != NULL);

    size_t size = 0;
    for(unsigned int  i = 0; i <= IMAGE_HEADER_SIZE; ++i)
      size |= (src.data[i + offset] & 1) << i;

    unsigned int index = 0;
    for(unsigned int  i = 0; i < size; ++i) {
      char c = 0;
      for(int j = 0; j < 8; ++j)
          c |= (src.data[i * 8 + IMAGE_HEADER_SIZE + j + offset] & 1) << j;
      
      //snprintf(buffer, size, "%c", c);
      printf("%c", c);
    }
}

extern unsigned int Font_size;

Image Image_text(const char* text)
{
  const size_t len = strlen(text);
  unsigned int lines = 1;

  for(size_t i = 0; i < len; ++i) {
    if(text[i] == '\n') ++lines;
  }

  Image r;
  Image_alloc(&r, Font_size * len, 1.2 * Font_size * lines, 1);
  Image_fill(&r, 0);

  unsigned int x     = 3; // pen position
  unsigned int y     = Font_size;
  unsigned int cropw = 0;

  Font_char fc;

  for(size_t i = 0; i < len; ++i)
  {
    if(text[i] == '\n') {
      x = 3;
      y += Font_size;
      continue;
    }

    Font_getc(&fc, text[i]);

    Image sub = {.w = fc.w, .h = fc.h, .c = 1, .data = fc.data};
    Image_overlay(r, sub, x, y - fc.h + (fc.h - fc.by) - (Font_size / 6));

    x += fc.advance;

    if(x > cropw)
      cropw = x;
  }

  Image_crop(&r, 0, 0, cropw, 1.2 * Font_size * lines);
  return r;
}

Image Image_wtext(const wchar_t* text)
{
  const size_t len = wcsnlen(text, ~0ull);
  unsigned int lines = 1;

  for(size_t i = 0; i < len; ++i) {
    if(text[i] == '\n') ++lines;
  }

  Image r;
  Image_alloc(&r, Font_size * len, 1.2 * Font_size * lines, 1);
  Image_fill(&r, 0);

  unsigned int x     = 3; // pen position
  unsigned int y     = Font_size;
  unsigned int cropw = 0;

  Font_char fc;

  for(size_t i = 0; i < len; ++i)
  {
    if(text[i] == '\n') {
      x = 3;
      y += Font_size;
      continue;
    }

    Font_getc(&fc, text[i]);

    Image sub = {.w = fc.w, .h = fc.h, .c = 1, .data = fc.data};
    Image_overlay(r, sub, x, y - fc.h + (fc.h - fc.by) - (Font_size / 6));

    x += fc.advance;

    if(x > cropw)
      cropw = x;
  }

  Image_crop(&r, 0, 0, cropw, 1.2 * Font_size * lines);
  return r;
}
