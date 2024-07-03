main: main.cpp src/image.cpp src/image.hpp
	g++ -Wall -I/usr/include/freetype2/ main.cpp src/image.cpp src/font.cpp -o main -llogger -lfreetype
