#include <exception>

#include <gtest/gtest.h>

#include "parser.h"

TEST(parser_error, throw_exception)
{
    try
    {
        throw paint::parse_error("ASDFGHJKL");
    }
    catch (paint::parse_error &e)
    {
        ASSERT_STREQ("ASDFGHJKL", e.error_substring());
    }

    try
    {
        throw paint::parse_error("ASDFGHJKL");
    }
    catch (std::exception &e)
    {
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}