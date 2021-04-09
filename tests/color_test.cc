#include <gtest/gtest.h>
#include "color.h"

TEST(colorRGB565, colorConversion)
{
    paint::ColorRGB888 rgb888(42, 84, 127);
    paint::ColorRGB565 rgb565(rgb888);
    paint::ColorRGB565 *p_rgb565 = new paint::ColorRGB565(0, 0, 0);
    p_rgb565->SetColor(rgb888);
    GTEST_ASSERT_EQ(paint::ColorRGB565((uint8_t)42 >> 3, (uint8_t)84 >> 2, (uint8_t)127 >> 3), rgb565);
    GTEST_ASSERT_EQ(paint::ColorRGB565((uint8_t)42 >> 3, (uint8_t)84 >> 2, (uint8_t)127 >> 3), *p_rgb565);
    delete p_rgb565;
}

TEST(colorRGB888, colorConversion)
{
    paint::ColorRGB565 rgb565(31, 63, 7);
    paint::ColorRGB888 rgb888(rgb565);
    paint::ColorRGB888 *p_rgb888 = new paint::ColorRGB888(0, 0, 0);
    p_rgb888->SetColor(rgb565);
    GTEST_ASSERT_EQ(paint::ColorRGB888((uint8_t)31 << 3, (uint8_t)63 << 2, (uint8_t)7 << 3), rgb888);
    GTEST_ASSERT_EQ(paint::ColorRGB888((uint8_t)31 << 3, (uint8_t)63 << 2, (uint8_t)7 << 3), *p_rgb888);
    delete p_rgb888;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}