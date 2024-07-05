#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <vector>

//check deps/ folder in https://github.com/Mostafa-Khab/gfx-project.git
#include <logger.hpp>

#include "font.hpp"
#include "image.hpp"

namespace gfx
{

  image::image(): width(0), height(0), channels(0), data(nullptr)
  {
  }

  image::image(std::string file)
  {
    if(!load(file))
      Log::error("failed to load requested image: " + file);
  }

  image::image(const image& img)
  {
    create(img.width, img.height, img.channels, img.data);
  }

  image::image(image&& img)
  {
    *this = img;
  }

  image::~image()
  {
    this->free();
  }

  bool image::load(std::string file)
  {
    data = reinterpret_cast<std::uint8_t*>(stbi_load(file.c_str(), &width, &height, &channels, 0));

    if(data)
      m_loaded = true;

    return data;
  }

  std::string image_format(std::string file);
  bool image::save(std::string file)
  {
    if(!channels || !width || !height)
    {
      Log::error("trying to create() a zero sized image");
      return false;
    }

    if(!data)
    {
      Log::error("trying to save() an image with a nullptr!!");
      return false;
    }

    std::string ext = image_format(file);
    int success = 1;

    if(ext == "png")
      success = stbi_write_png(file.c_str(), width, height, channels, data, width * channels);
    else if(ext == "jpg")
      success = stbi_write_jpg(file.c_str(), width, height, channels, data, 100);
    else if(ext == "tga")
      success = stbi_write_tga(file.c_str(), width, height, channels, data);
    else if(ext == "bmp")
      success = stbi_write_tga(file.c_str(), width, height, channels, data);
    else {
      Log::error("trying to save an image with unsupported format: " + ext);
      return false;
    }

    if(!success)
      Log::error("it seem that stbi_write_" + ext + " has failed");

    return success;
  }

  bool image::create(int w, int h, int c)
  {
    if(!c || !w || !h)
    {
      Log::error("trying to create() a zero sized image");
      return false;
    }

    this->free();

    data     = new std::uint8_t[w * h * c]{0};
    width    = w;
    height   = h;
    channels = c;

    std::memset(data, 255, w * h * c);
    return true;
  }

  bool image::create(int w, int h, int c, std::uint8_t* d)
  {
    if(!c || !w || !h)
    {
      Log::error("trying to create() a zero sized image");
      return false;
    }

    if(!d)
    {
      Log::error("create() tried to copy pixels from a nullptr");
      return false;
    }

    this->free();

    data     = new std::uint8_t[w * h * c]{0};
    width    = w;
    height   = h;
    channels = c;

    std::memcpy(data, d, w * h * c);
    return true;
  }

  bool image::create(std::wstring str, font& f)
  {
    if(!f.m_loaded || f.m_done)
    {
      Log::error("using invalid font");
      return false;
    }

    int lines_count = 1;
    for(unsigned int i = 0; i < str.size(); ++i)
    {
      if(str[i] == '\n')
        ++lines_count;
    }

    image text; 
    text.create(str.size() * f.size, 1.2 * f.size * lines_count, 1);
    text.mask(0,0,0);
    int pen = 3;
    int max_width = 0;

    int y = f.size;
    for(unsigned int i = 0; i < str.size(); ++i)
    {
      if(str[i] == '\n')
      {
        pen = 3;
        y += f.size;
        continue;
      }

      auto ch = f.get(str[i]);
      image sub;
      sub.create(ch.w, ch.h, 1, ch.data);
      text.overlay(sub, pen, y - ch.h + (ch.h - ch.bearing_y) - (f.size / 6));
      pen += ch.advance;
      if(pen > max_width)
        max_width = pen;
    }

    text.crop(0, 0, max_width, 1.2 * f.size * lines_count);
    *this = std::move(text);
    return true;
  }

  bool image::create(std::string str, font& f)
  {
    return create(std::wstring(str.begin(), str.end()), f);
  }

  void image::free()
  {
    if(!data)
      return;

    if(m_loaded)
      stbi_image_free(data);
    else
      delete[] data;

    data = nullptr;
  }

  long image::size() const
  {
    return width * height * channels;
  }

  //this section will be more related to effects on images.
  image& image::mask(float r, float g, float b, float a)
  {
    for(int i = 0; i < size(); i += channels)
    {
      data[i] *= r;
      if(channels > 1) data[i + 1] *= g;
      if(channels > 2) data[i + 2] *= b;
      if(channels > 3) data[i + 3] *= a;
    }

    return *this;
  }

  //this function requires some checks. do them to avoid segfaults.
  image& image::overlay(const image& img, int x, int y)
  {

    for(int i = 0; i < img.height; ++i)
    {
      if(i + y < 0) continue;
      if(i + y >= height) break;

      for(int j = 0; j < img.width; ++j)
      {
        if(j + x >= width) break;
        if(j + x < 0) continue;

        std::uint8_t* d = &data[((i + y) * width + (j + x)) * channels];
        std::uint8_t* s = &img.data[(i * img.width + j) * img.channels];

        float s_alpha = (img.channels < 4)? 1 : s[3] / 255.f;
        //float d_alpha = (    channels < 4)? 1 : d[3] / 255.f;

        for(int c = 0; c < img.channels && c < channels; ++c)
        {
          d[c] = s[c] * s_alpha + d[c] * (1.f - s_alpha);
        }
      }
    }
    return *this;
  }

  image& image::crop(int x, int y, int w, int h)
  {
    if(x < 0 || y < 0)
      return *this;

    image cropped;
    cropped.create(w, h, channels);

    for(int i = 0; i < h; ++i)
    {
      if(i + y >= height) break;
      for(int j = 0; j < w; ++j)
      {
        if(j + x >= width) break;

        std::memcpy(
            &cropped.data[(i * w + j) * channels],
            &data[((i + y) * width + (j + x)) * channels],
            channels
         );
      }
    }

    this->free();
    *this = cropped;
    return *this;
  }

  image& image::grayscale_avg()
  {
    if(channels < 3)
      Log::error("this image is already in grayscale");

    for(int i = 0; i < width * height * channels; i += channels)
    {
      int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
      std::memset(&data[i], gray, 3);
    }

    return *this;

  }

  image& image::grayscale_lum()
  {
    if(channels < 3)
      Log::error("this image is already in grayscale");

    for(int i = 0; i < width * height * channels; i += channels)
    {
      int gray = (0.2126f * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2]) / 3;
      std::memset(&data[i], gray, 3);
    }

    return *this;
  }

  image& image::flip_x()
  {
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width / 2; ++j)
      { 
        auto l = &data[(i * width + j) * channels];
        auto r = &data[(i * width + (width - j - 1)) * channels];
        for(int c = 0; c < channels; ++c)
          std::swap(l[c], r[c]);
      }
    }

    return *this;
  }

  image& image::flip_y()
  {
    for(int i = 0; i < height / 2; ++i)
    {
      for(int j = 0; j < width; ++j)
      { 
        auto u = &data[(i * width + j) * channels];
        auto d = &data[((height - i - 1) * width + j) * channels];
        for(int c = 0; c < channels; ++c)
          std::swap(u[c], d[c]);
      }
    }

    return *this;
  }

  image& image::flip_channel_x(std::uint8_t channel)
  {
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width / 2; ++j)
      { 
        auto l = &data[(i * width + j) * channels];
        auto r = &data[(i * width + (width - j - 1)) * channels];
        std::swap(l[channel], r[channel]);
      }
    }

    return *this;
  }

  image& image::flip_channel_y(std::uint8_t channel)
  {
    for(int i = 0; i < height / 2; ++i)
    {
      for(int j = 0; j < width; ++j)
      { 
        auto u = &data[(i * width + j) * channels];
        auto d = &data[((height - i - 1) * width + j) * channels];
        std::swap(u[channel], d[channel]);
      }
    }

    return *this;
  }

// #define CHECK_THEN_ADD(X, Y, Z)                           \
  // if((X) < height && (X) > 0 && (Y) < width && (Y) > 0)   \
    // sum += data[((X) * width + (Y)) * channels + c] * (Z);

//will this have any performance gain?
//future me, I'm sorry for this mess, but I'm hungry for performance even if small.

#define CHECK_THEN_ADD(X, Y, Z)                           \
    sum += data[(((X) >= height? height - 1 : (X) < 0? 0 : (X)) * width + ((Y) >= width? width - 1 : (Y) < 0? 0 : (Y))) * channels + c] * (Z);

  //kernal dimentions should be inversed??
  image& image::apply_kernel(std::vector<std::vector<float>>& kernel, float div)
  {
    image img;
    img.create(width, height, channels);
    if(img.data == nullptr)
      Log::error("failed to allocate memory (malloc). wtf");

    //NOTE: the commented version won't work. because unsigned numbers can't be negative.
    //as maxv is used as a negative number in y and x for loop, THAT WAS NEAR!!

    //auto maxv = (kernel.size() - 1) / 2;
    int maxv = (kernel.size() - 1) / 2;

    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j)
      {
        for(int c = 0; c < channels ; ++c)
        {
          long sum = 0;
          for(int y = -maxv; y <= maxv; ++y)
            for(int x = -maxv; x <= maxv; ++x)
              CHECK_THEN_ADD(i + y, j + x, kernel[y + maxv][x + maxv]);


          //CHECK_THEN_ADD(i - 2, j - 2, kernal[0][0]);
          //CHECK_THEN_ADD(i - 2, j - 1, kernal[0][1]);
          //CHECK_THEN_ADD(i - 2, j - 0, kernal[0][2]);
          //CHECK_THEN_ADD(i - 2, j + 1, kernal[0][3]);
          //CHECK_THEN_ADD(i - 2, j + 2, kernal[0][4]);

          //CHECK_THEN_ADD(i - 1, j - 2, kernal[1][0]);
          //CHECK_THEN_ADD(i - 1, j - 1, kernal[1][1]);
          //CHECK_THEN_ADD(i - 1, j - 0, kernal[1][2]);
          //CHECK_THEN_ADD(i - 1, j + 1, kernal[1][3]);
          //CHECK_THEN_ADD(i - 1, j + 2, kernal[1][4]);

          //CHECK_THEN_ADD(i - 0, j - 2, kernal[2][0]);
          //CHECK_THEN_ADD(i - 0, j - 1, kernal[2][1]);
          //CHECK_THEN_ADD(i - 0, j - 0, kernal[2][2]);
          //CHECK_THEN_ADD(i - 0, j + 1, kernal[2][3]);
          //CHECK_THEN_ADD(i - 0, j + 2, kernal[2][4]);

          //CHECK_THEN_ADD(i + 1, j - 2, kernal[3][0]);
          //CHECK_THEN_ADD(i + 1, j - 1, kernal[3][1]);
          //CHECK_THEN_ADD(i + 1, j - 0, kernal[3][2]);
          //CHECK_THEN_ADD(i + 1, j + 1, kernal[3][3]);
          //CHECK_THEN_ADD(i + 1, j + 2, kernal[3][4]);

          //CHECK_THEN_ADD(i + 2, j - 2, kernal[4][0]);
          //CHECK_THEN_ADD(i + 2, j - 1, kernal[4][1]);
          //CHECK_THEN_ADD(i + 2, j - 0, kernal[4][2]);
          //CHECK_THEN_ADD(i + 2, j + 1, kernal[4][3]);
          //CHECK_THEN_ADD(i + 2, j + 2, kernal[4][4]);
          
          img.data[(i * width + j) * channels + c] = sum / div;
        }
      }
    }

    *this = std::move(img);
    return *this;
  }

  image& image::operator= (const image& img)
  {
    create(img.width, img.height, img.channels, img.data);
    return *this;
  }

  image& image::operator= (image&& img)
  {
    if(!img.channels || !img.width || !img.height)
    {
      Log::error("trying to create() a zero sized image");
      return *this;
    }

    if(!img.data)
    {
      Log::error("create() tried to copy pixels from a nullptr");
      return *this;
    }
    
    if(data)
      this->free();

    data     = img.data;
    width    = img.width;
    height   = img.height;
    channels = img.channels;

    //img.data mustn't be null to be freed!, nice trick.
    img.data = nullptr;
    Log::info("copy construct from rvalue, 1 heap allocation saved");
    return *this;
  }

  void image::dump(std::ostream& os)
  {
    for(int i = 0; i < width * height * channels; ++i)
    {
      if(i % channels == 0)
        os << ' ';

      if(!data[i])
        os << "00";
      else
        os << std::hex << (unsigned int)data[i];
    }
  }

  //this section isn't related too much to an image.
  std::string image_format(std::string file)
  {
    //let's try rbegin if this shit worked
    return std::string(std::find(file.begin(), file.end(), '.') + 1, file.end());
  }
}
