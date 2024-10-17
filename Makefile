main: main.c gfx_image gfx_font
	gcc -fsanitize=address -g main.c build/gfx_image.o build/gfx_font.o -o build/main -lm -I/usr/include/freetype2 -I/usr/include/libpng16 -lfreetype


gfx_image: src/image.c src/image.h
	gcc -g src/image.c -c -o build/gfx_image.o
	ar cq build/libImage.a build/gfx_image.o 

gfx_font: src/font.c src/font.h
	bear -- gcc -g src/font.c -c -o build/gfx_font.o -I/usr/include/freetype2 
	ar cq build/libImage.a build/gfx_font.o

clean: build/
	rm -r build/*

test: tests/test.c
	gcc -g test.c build/gfx_image.o build/gfx_font.o -o build/test -lm

#change install path to ur liking
install: main
	install build/libImage.a /usr/local/lib/
	install src/image.h src/log.h /usr/local/include
