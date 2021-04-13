include_directories(${PARENT_PATH_OF_GTEST}/googltest/googltest/include)

# add_executable(parse_test "./parser_test.cpp")
# target_include_directories(parse_test PUBLIC "${Paint_SOURCE_DIR}/inc")

set(PaintSources ../src/color.cc ../src/parser.cc)

add_executable(parser_test "./parser_test.cc" ${PaintSources})
add_executable(parser_line_command "./parser_line_test.cc" ${PaintSources})
add_executable(color_test "./color_test.cc" ${PaintSources})
include_directories("${Paint_SOURCE_DIR}/inc")

# set_target_properties(parse_test color_test PROPERTIES EXCLUDE_FROM_ALL 1)

# add_test(ParseTest parse_test)
include(GoogleTest)
gtest_discover_tests(parser_test)
gtest_discover_tests(parser_line_command)
gtest_discover_tests(color_test)

target_link_libraries(parser_test gtest)
target_link_libraries(parser_line_command gtest)
target_link_libraries(color_test gtest)