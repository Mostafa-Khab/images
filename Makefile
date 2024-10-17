all: main test

main: main.c lib
	gcc main.c build/libImage.a -o build/main -lm -I/usr/include/freetype2 -lfreetype

lib: image.o font.o
	ar cq build/libImage.a build/image.o
	ar cq build/libImage.a build/font.o

image.o: src/image.c
	gcc -c src/image.c -o build/image.o -lm -I/usr/include/freetype2

font.o: src/font.c
	gcc -c src/font.c -o build/font.o -lm -I/usr/include/freetype2 -lfreetype

test: tests/test.c
	gcc -g tests/test.c src/*.c -o build/test -I/usr/include/freetype2 -lm -lfreetype

#change install path to ur liking
install: main
	install build/libImage.a /usr/local/lib/
	install src/*.h /usr/local/include
	
clean: build/
	rm -r build/*
