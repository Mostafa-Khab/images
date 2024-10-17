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
