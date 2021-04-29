# Use main.cpp to call and debug your code

# FILE(GLOB SRCFILES *.cc)

# get_filename_component(painter ./painter.cc ABSOLUTE)
# list(APPEND PaintSources ${painter})

# get_filename_component(parser ./parser.cc ABSOLUTE)
# list(APPEND PaintSources ${parser})

# get_filename_component(paint ./paint.cc ABSOLUTE)
# list(APPEND PaintSources ${paint})

# get_filename_component(image ./image.cc ABSOLUTE)
# list(APPEND PaintSources ${image})

# get_filename_component(image_png ./image_png.cc ABSOLUTE)
# list(APPEND PaintSources ${image_png})

# get_filename_component(image_bmp ./image_bmp.cc ABSOLUTE)
# list(APPEND PaintSources ${image_bmp})

# get_filename_component(color_rgb888 ./color_rgb888.cc ABSOLUTE)
# list(APPEND PaintSources ${color_rgb888})

# get_filename_component(color_rgb565 ./color_rgb565.cc ABSOLUTE)
# list(APPEND PaintSources ${color_rgb565})

# get_filename_component(color_grayscale ./color_grayscale.cc ABSOLUTE)
# list(APPEND PaintSources ${color_grayscale})

# get_filename_component(color_bw ./color_bw.cc ABSOLUTE)
# list(APPEND PaintSources ${color_bw})

#set(PaintSources ${painter} ${parser} ${paint} ${image} ${image_png} ${image_bmp} ${color_grayscale} ${color_rgb888} ${color_bw} ${color_rgb565} PARENT_SCOPE)

add_executable(Empty_Main ./empty_main.cc ${PaintSources})
add_executable(Paint ./main.cc  ${PaintSources})
add_executable(App ./app.cc ${PaintSources})

target_include_directories(Empty_Main PUBLIC "${Paint_SOURCE_DIR}/inc")
target_include_directories(Paint PUBLIC "${Paint_SOURCE_DIR}/inc")
target_include_directories(App PUBLIC "${Paint_SOURCE_DIR}/inc")
