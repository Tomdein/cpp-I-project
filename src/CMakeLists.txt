# Use main.cpp to call and debug your code

FILE(GLOB SRCFILES *.cc)

set(PaintSources ./main.cc ./color.cc ./parser.cc)

add_executable(Paint ${PaintSources})

target_include_directories(Paint PUBLIC "${Paint_SOURCE_DIR}/inc")
