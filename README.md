# simple image manipulation library [C version]
- This is a small library that can do some useful image manipulations.
- C implementation still lacks font rendering [will exist in future]

### what can you do?
- some manipulations like crop, color mask, overlay, creating plain ones, embedding data, etc...

### deps
- stb_image.h and stb_image_write.h from [https://github.com/nothings/stb.git](https://github.com/nothings/stb.git).

### simple example
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
- compile with
```bash
gcc main.cpp -o main -lImage -lm
```
- make sure to put this directory in your include path.

### NOTE:
  This small project was made for learning purposes, so don't overrate it :smile:.

