# Use main.cpp to call and debug your code

FILE(GLOB SRCFILES *.cc)

add_executable(Paint "./main.cc" "./color.cc")
# add_executable(Paint $(SRCFILES))

target_include_directories(Paint PUBLIC "${Paint_SOURCE_DIR}/inc")
