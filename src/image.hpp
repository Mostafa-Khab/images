/*********************************
 * author     : mostafa khaled
 * date       : Mon Jul  1 08:39:42 PM EEST 2024
 * desc       : 
 ********************************/
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <sstream>
#include <cstdint>
#include <vector>

#include "font.hpp"

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

      image& apply_kernel(std::vector<std::vector<float>>& kernal, float div = 25.f);

      //embed an extract an image from another
      //layer is the bit number from the color.
      //layer in embed and extract don't need to match, but it would be better.
      image& embed(const image& img, const int x, const int y, uint8_t layer = 7);
      image& extract(const image& img, const int x, const int y, uint8_t layer = 7);

      image& embed(const std::string& str, const int start = 0);
      std::string extract(int start = 0) const; //for small string
      void extract(std::stringstream& ss, const int start = 0) const; //for large string

      image& operator= (const image& img);
      image& operator= (image&& img);

      void dump(std::ostream& os);
      static bool valid(std::string ext);
      static void flip_y_on_load(bool b = true);

    public:
      int width, height, channels;
      std::uint8_t*  data;
    private:
      bool m_loaded = false;;
  };

  std::string ext(std::string file);

  typedef std::vector<std::vector<float>> kernel_t;
}

#endif /* !IMAGE_HPP */
