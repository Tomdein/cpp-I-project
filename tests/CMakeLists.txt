include_directories(${PARENT_PATH_OF_GTEST}/googltest/googltest/include)

# add_executable(parse_test "./parser_test.cpp")
# target_include_directories(parse_test PUBLIC "${Paint_SOURCE_DIR}/inc")

add_executable(parse_test "./parser_test.cc")
add_executable(color_test "./color_test.cc")
include_directories("${Paint_SOURCE_DIR}/inc")

# add_test(ParseTest parse_test)
include(GoogleTest)
gtest_discover_tests(parse_test)
gtest_discover_tests(color_test)

target_link_libraries(parse_test gtest)
target_link_libraries(color_test gtest)