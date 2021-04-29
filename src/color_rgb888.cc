#include <cmath>

#include "colors.h"

namespace paint
{
    ColorRGB565 ColorRGB888::ToRGB565() const
    {
        return ColorRGB565(pixel_.r >> 3, pixel_.g >> 2, pixel_.b >> 3);
    }

    ColorRGB888 ColorRGB888::ToRGB888() const
    {
        return ColorRGB888(*this);
    }

    ColorGrayscale ColorRGB888::ToGrayscale() const
    {
        //https://stackoverflow.com/questions/14330/rgb-to-monochrome-conversion
        return ColorGrayscale(std::round(0.2125f * pixel_.r + 0.7154f * pixel_.g + 0.0721f * pixel_.b));
    }

    ColorBW ColorRGB888::ToBW() const
    {
        return ColorBW(pixel_.r > 127 || pixel_.g > 127 || pixel_.b > 127);
    }
}