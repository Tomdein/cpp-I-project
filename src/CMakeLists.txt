# Use main.cpp to call and debug your code

# FILE(GLOB SRCFILES *.cc)

add_executable(Empty_Main ./empty_main.cc ${PaintSources})
add_executable(Main ./main.cc  ${PaintSources})
add_executable(App ./app.cc ${PaintSources})
# target_include_directories(App PUBLIC "${Paint_SOURCE_DIR}/inc" ${CRCpp_HEADER})
target_link_libraries(App PUBLIC ${ZLIB_LIBRARY})

target_include_directories(Empty_Main PUBLIC "${Paint_SOURCE_DIR}/inc")
target_include_directories(Main PUBLIC "${Paint_SOURCE_DIR}/inc")
target_include_directories(App PUBLIC "${Paint_SOURCE_DIR}/inc")
