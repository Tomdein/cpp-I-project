# Use main.cpp to call and debug your code

FILE(GLOB SRCFILES *.cc)

set(PaintSources ./color.cc ./parser.cc ./paint.cc ./image_png.cc ./image_bmp.cc)

add_executable(Paint ./main.cc  ${PaintSources})
add_executable(App ./app.cc ${PaintSources})

target_include_directories(Paint PUBLIC "${Paint_SOURCE_DIR}/inc")
target_include_directories(App PUBLIC "${Paint_SOURCE_DIR}/inc")
