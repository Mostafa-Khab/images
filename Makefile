main: main.c gfx_image
	gcc main.c build/gfx_image.o -o build/main -lm

gfx_image: src/*.c src/*.h
	gcc src/*.c -c -o build/gfx_image.o -lm

clean: build/
	rm -r build/*

test: tests/test.c
	gcc test.c build/gfx_image.o -o build/test -lm
