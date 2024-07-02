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
  class font;

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
      bool create(std::string str, font& f);
      bool create(std::wstring str, font& f);

      long size() const;

      void free();

      image& mask(float r, float g, float b, float a = 1.f);
      image& overlay(const image& img, int x, int y);
      //image& overlay(std::wstring, font& f);

      image& crop(int x, int y, int w, int h);

      image& grayscale_avg();
      image& grayscale_lum();

      image& flip_x();
      image& flip_y();

      image& flip_channel_x(std::uint8_t channel); //flips only a certian channel in x.
      image& flip_channel_y(std::uint8_t channel); //flips only a certian channel in y.

      image& operator= (const image& img);
      image& operator= (image&& img);

      void dump(std::ostream& os);

    public:
      int width, height, channels;
      std::uint8_t*  data;
  };
}

#endif /* !IMAGE_HPP */
