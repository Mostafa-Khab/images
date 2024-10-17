cmake_minimum_required(VERSION 3.25)
project(IMAGES
  LANGUAGES C
  VERSION 2.0
)

file(GLOB SOURCES "src/*")

add_library(Image ${SOURCES})
add_executable(main main.c)

target_link_libraries(Image PUBLIC freetype m)
target_link_libraries(main PUBLIC Image freetype m)
target_include_directories(Image PUBLIC /usr/include/freetype2/)
target_include_directories(main PUBLIC /usr/include/freetype2/)

install(
  TARGETS Image DESTINATION lib
)

install(
  FILES src/font.h src/image.h
  DESTINATION include
)
