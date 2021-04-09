#include <gtest/gtest.h>
#include "color.h"

TEST(colorRGB888, colorConversion)
{
    // paint::ColorRGB888 rgb888(42, 84, 127);
    // paint::ColorRGB565 rgb565(rgb888);
    // GTEST_ASSERT_EQ(rgb565., 42);
    GTEST_ASSERT_EQ(0, 42);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}