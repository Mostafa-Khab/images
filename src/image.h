/*********************************
 * author     : mostafa khaled
 * date       : Wed Oct 16 02:46:45 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef IMAGE_H
#define IMAGE_H

//NOTE: This is the C version image.hpp is the cpp version

#include <stdint.h>
#include <stdlib.h>

#include "font.h"

typedef struct Image
{
  unsigned int w, h, c; // width, height, channels
  unsigned char* data;
} Image;

void Image_alloc(Image* img, unsigned int w, unsigned int h, unsigned int c);
void Image_load(Image* img, const char* filepath);
void Image_save(Image img, const char* filepath);

void Image_fill(Image* img, uint8_t color);

void Image_free(Image* img);

void Image_mask(Image img, float* mask);
void Image_overlay(Image bg, Image fg, int x, int y);

void Image_crop(Image* img, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

void Image_grayscale_avg(Image img);
void Image_grayscale_lum(Image img);

void Image_flip_x(Image img);
void Image_flip_y(Image img);

void Image_flip_channel_x(Image img, uint8_t channel); //flips only a certian channel in x.
void Image_flip_channel_y(Image img, uint8_t channel); //flips only a certian channel in y.

// void Image_convolve(kernel, float div); for later

//embed an extract an image from another
//layer is the bit number from the color.
//layer in embed and extract don't need to match, but it would be better.

void Image_embed(Image bg, const Image fg, const int x, const int y, uint8_t layer);
void Image_extract(Image bg, const Image fg, const int x, const int y, uint8_t layer);

void Image_embed_text(Image dst, const char* text, const unsigned int offset);
void Image_extract_text(Image src, unsigned int offset);

Image Image_text(const char* text);

#endif /* !IMAGE_H */
