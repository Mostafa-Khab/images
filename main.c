#include <stdio.h>
#include "src/image.h"

int main()
{
  const char* text =
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    "hallo, mein freund\n"
    ;

  Image bird;
  Image_load(&bird, "/sasa/Pictures/bird64.png");
  Image_embed_text(bird, text, 0);

  Image_save(bird, "./embed.png");

  char buff[512] = {0};
  Image_extract_text(bird,0);

  printf("%s \n", buff);

  Image_free(&bird);

  return 0;
}
