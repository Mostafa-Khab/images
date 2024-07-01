/*********************************
 * author     : mostafa khaled
 * date       : Mon Jul  1 08:39:42 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <cstdint>

namespace gfx
{
  class image
  {
    public:
      image();
      image(std::string file);
      image(const image& img);
      image(image&& img);

     ~image();

      bool load(std::string file);
      bool save(std::string file);

      bool create(int width, int height, int channels);
      bool create(int width, int height, int channels, std::uint8_t* data);

      long size() const;

      void free();

      void mask(float r, float g, float b, float a = 1.f);
      void overlay(const image& img, int x, int y);

      image crop(int x, int y, int w, int h);

      image& operator= (const image& img);
      image& operator= (image&& img);

    public:
      int width, height, channels;
      std::uint8_t*  data;
  };
}

#endif /* !IMAGE_HPP */
