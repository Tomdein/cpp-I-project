#include <cmath>

#include "colors.h"

namespace paint
{
    ColorRGB565 ColorRGB888::ToRGB565() const
    {
        return ColorRGB565(pixel_.r >> 3, pixel_.g >> 2, pixel_.b >> 3);
    }

    ColorBGR565 ColorRGB888::ToBGR565() const
    {
        return ColorBGR565(pixel_.b >> 3, pixel_.g >> 2, pixel_.r >> 3);
    }

    ColorRGB888 ColorRGB888::ToRGB888() const
    {
        return ColorRGB888(*this);
    }

    ColorBGR888 ColorRGB888::ToBGR888() const
    {
        return ColorBGR888(pixel_.b, pixel_.g, pixel_.r);
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