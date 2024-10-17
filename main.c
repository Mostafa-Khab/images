#include <stdio.h>

#include "log.h"
#include "src/image.h"
#include "src/font.h"

#define print(X) \
  printf(#X" = %d\n", (X));

int main()
{
  Font_init();
  Font_load("/usr/share/fonts/truetype/hack/Hack-Regular.ttf");
  Font_setSize(48);

  Image text = Image_text("just go away");
  Image_save(text, "text.png");

  Image_free(&text);
  Font_terminate();
  return 0;
}
