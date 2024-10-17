#include <stdio.h>

#include "log.h"
#include "src/image.h"
#include "src/font.h"

#define print(X) \
  printf(#X" = %d\n", (X));

int main()
{
  Font_char fc = {0};

  Font_init();
  Font_load("/usr/share/fonts/truetype/hack/Hack-Regular.ttf");
  Font_setSize(0, 48);
  Font_getc(&fc, 97);

  print(fc.w);
  print(fc.h);
  print(fc.advance);
  print(fc.bx);
  print(fc.by);

  Font_terminate();
  return 0;
}
