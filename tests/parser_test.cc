#include <gtest/gtest.h>
#include "parser.h"

TEST(parser, basic_parse)
{
    GTEST_ASSERT_EQ(1, 42);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}