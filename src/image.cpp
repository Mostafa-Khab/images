#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include <cstdint>
#include <cstring>
#include <algorithm>

//check deps/ folder in https://github.com/Mostafa-Khab/gfx-project.git
#include <logger.hpp>

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

  void image::free()
  {
    if(!data)
      return;

    stbi_image_free(data);
    data = nullptr;
  }

  long image::size() const
  {
    return width * height * channels;
  }

  //this section will be more related to effects on images.
  void image::mask(float r, float g, float b, float a)
  {
    for(int i = 0; i < size(); i += channels)
    {
      data[i] *= r;
      if(channels > 1) data[i + 1] *= g;
      if(channels > 2) data[i + 2] *= b;
      if(channels > 3) data[i + 3] *= a;
    }
  }

  //this function requires some checks. do them to avoid segfaults.
  void image::overlay(const image& img, int x, int y)
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

    data     = img.data;
    width    = img.width;
    height   = img.height;
    channels = img.channels;

    img.data = nullptr;
    Log::info("copy construct from rvalue, 1 heap allocation saved");
    return *this;
  }

  //this section isn't related too much to an image.
  std::string image_format(std::string file)
  {
    //let's try rbegin if this shit worked
    return std::string(std::find(file.begin(), file.end(), '.') + 1, file.end());
  }
}
