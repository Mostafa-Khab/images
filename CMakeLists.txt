cmake_minimum_required(VERSION 3.18..3.29)
project(
  IMAGES
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_INSTALL_PREFIX /usr/local/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

include_directories(/usr/include/freetype2/)

add_library(gfx_image src/image.cpp src/font.cpp src/image.hpp src/font.hpp)
target_link_libraries (gfx_image PUBLIC  freetype logger)
target_compile_options(gfx_image PUBLIC "-Wall" "-g")

add_executable(main main.cpp)
target_link_options   (main PUBLIC "-Wl,-heap=1000000000")
target_compile_options(main PUBLIC "-Wall" "-g")
target_link_libraries (main PUBLIC gfx_image)

install(
  TARGETS gfx_image
  DESTINATION lib
)

install(
  FILES src/font.hpp src/image.hpp
  DESTINATION include
)

