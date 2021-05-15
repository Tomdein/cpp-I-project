#include <cmath>

#include "colors.h"

namespace paint
{
    ColorRGB565 ColorBGR888::ToRGB565() const
    {
        return ColorRGB565(pixel_.r >> 3, pixel_.g >> 2, pixel_.b >> 3);
    }

    ColorBGR565 ColorBGR888::ToBGR565() const
    {
        return ColorBGR565(pixel_.b >> 3, pixel_.g >> 2, pixel_.r >> 3);
    }

    ColorRGB888 ColorBGR888::ToRGB888() const
    {
        return ColorRGB888(pixel_.r, pixel_.g, pixel_.b);
    }

    ColorBGR888 ColorBGR888::ToBGR888() const
    {
        return ColorBGR888(*this);
    }

    ColorGrayscale ColorBGR888::ToGrayscale() const
    {
        //https://stackoverflow.com/questions/14330/rgb-to-monochrome-conversion
        return ColorGrayscale(std::round(0.2125f * pixel_.r + 0.7154f * pixel_.g + 0.0721f * pixel_.b));
    }

    ColorBW ColorBGR888::ToBW() const
    {
        return ColorBW(pixel_.r > 127 || pixel_.g > 127 || pixel_.b > 127);
    }
}