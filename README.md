# simple image manipulation library [C version]
- This is a small library that can do some useful image manipulations.
- Also it supports font rendering

### what can you do?
- some manipulations like crop, color mask, overlay, creating plain ones, embedding data, etc...
- create images from text

### deps
- stb_image.h and stb_image_write.h from [https://github.com/nothings/stb.git](https://github.com/nothings/stb.git).
- also [freetype2](https://freetype.org)

## Examples

#### Grayscale

```c
#include "src/image.h"

int main()
{
  Image img;
  Image_load(&img, "../assets/bird64.png);
  Image_grayscale_avg(img);
  Image_save(img, "gray.png");

  return 0;
}
```

#### Font Rendering

```c
#include "src/image.h"
#include "src/font.h"

int main()
{
  Font_init();
  Font_load("../assets/hack.ttf");
  Font_setSize(48);

  Image text = Image_text("hello Freetype ");
  Image_save(text, "text.png");

  Image_free(&text);
  Font_terminate();
  return 0;
}
```

- build this library then install it.
- you can build your simple project with:

```bash
$ gcc main.cpp -o main -lImage lfreetype -lm -I/usr/include/freetype2
```
- make sure to put this directory in your include path.

### NOTE:
  This small project was made for learning purposes, so don't overrate it :smile:.

