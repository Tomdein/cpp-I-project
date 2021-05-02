# Use main.cpp to call and debug your code

add_executable(Empty_Main ./empty_main.cc ${PaintSources})
# target_include_directories(Empty_Main PUBLIC "${Paint_SOURCE_DIR}/inc" ${CRCpp_HEADER})
target_link_libraries(Empty_Main PUBLIC ${ZLIB_LIBRARY})

add_executable(Paint ./main.cc  ${PaintSources})
# target_include_directories(Paint PUBLIC "${Paint_SOURCE_DIR}/inc" ${CRCpp_HEADER})
target_link_libraries(Paint PUBLIC ${ZLIB_LIBRARY})

add_executable(App ./app.cc ${PaintSources})
# target_include_directories(App PUBLIC "${Paint_SOURCE_DIR}/inc" ${CRCpp_HEADER})
target_link_libraries(App PUBLIC ${ZLIB_LIBRARY})

