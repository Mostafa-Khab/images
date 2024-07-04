main: main.cpp src/image.cpp src/image.hpp
	g++ -Wall -Wl,-heap=100000000 -I/usr/include/freetype2/ main.cpp src/image.cpp src/font.cpp -o main -llogger -lfreetype
