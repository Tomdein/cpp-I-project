cmake_minimum_required(VERSION 3.10.0)
project(Paint VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")


message("generator is set to ${CMAKE_GENERATOR}")

add_compile_options(-Wall -Wextra -pedantic)

include(CTest)
enable_testing()

get_filename_component(painter ./src/painter.cc ABSOLUTE)
list(APPEND PaintSources ${painter})

get_filename_component(parser ./src/parser.cc ABSOLUTE)
list(APPEND PaintSources ${parser})

get_filename_component(paint ./src/paint.cc ABSOLUTE)
list(APPEND PaintSources ${paint})

get_filename_component(image ./src/image.cc ABSOLUTE)
list(APPEND PaintSources ${image})

get_filename_component(image_png ./src/image_png.cc ABSOLUTE)
list(APPEND PaintSources ${image_png})

get_filename_component(image_bmp ./src/image_bmp.cc ABSOLUTE)
list(APPEND PaintSources ${image_bmp})

get_filename_component(image_iobmp ./src/image_iobmp.cc ABSOLUTE)
list(APPEND PaintSources ${image_iobmp})

get_filename_component(color_rgb888 ./src/color_rgb888.cc ABSOLUTE)
list(APPEND PaintSources ${color_rgb888})

get_filename_component(color_rgb565 ./src/color_rgb565.cc ABSOLUTE)
list(APPEND PaintSources ${color_rgb565})

get_filename_component(color_grayscale ./src/color_grayscale.cc ABSOLUTE)
list(APPEND PaintSources ${color_grayscale})

get_filename_component(color_bw ./src/color_bw.cc ABSOLUTE)
list(APPEND PaintSources ${color_bw})

message(${PaintSources})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(googletest) 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)