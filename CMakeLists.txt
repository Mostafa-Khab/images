cmake_minimum_required(VERSION 3.18..3.29)
project(
  IMAGES
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX "/usr/local/" CACHE STRING "path to install gfx_image")
set(FREETYPE2_PATH "/usr/include/freetype2/" CACHE STRING "path to freetype2 folder")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_SHARED_LIBS ON CACHE)

message("path to freetype2: ${FREETYPE2_PATH}")

add_library(gfx_image src/image.cpp src/font.cpp src/image.hpp src/font.hpp)
target_include_directories(gfx_image PUBLIC "${FREETYPE2_PATH}")

add_executable(main main.cpp)
target_link_options(main PRIVATE "-Wl,-rpath,/usr/local/lib/")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(gfx_image PRIVATE "-g" "-Wall" "-Wextra")
  target_compile_options(main PRIVATE "-g")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(NOT BUILD_SHARED_LIBS)
  target_link_options(main PUBLIC "-static")
  target_link_libraries(main PUBLIC gfx_image freetype png16 m z brotlidec brotlicommon -l:liblogger.a)
else()
  target_link_options(main PUBLIC "-shared")
  target_link_libraries(main PUBLIC gfx_image freetype)
endif(NOT BUILD_SHARED_LIBS)

install(
  TARGETS gfx_image
  DESTINATION lib
)

install(
  FILES src/font.hpp src/image.hpp
  DESTINATION include
)

