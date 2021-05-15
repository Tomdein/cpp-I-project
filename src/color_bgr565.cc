#include "colors.h"

namespace paint
{
    ColorRGB565 ColorBGR565::ToRGB565() const
    {
        return ColorRGB565(pixel_.r, pixel_.g, pixel_.b);
    }

    ColorBGR565 ColorBGR565::ToBGR565() const
    {
        return ColorBGR565(*this);
    }

    ColorRGB888 ColorBGR565::ToRGB888() const
    {
        return ColorRGB888(pixel_.r << 3, pixel_.g << 2, pixel_.b << 3);
    }

    ColorBGR888 ColorBGR565::ToBGR888() const
    {
        return ColorBGR888(pixel_.b << 3, pixel_.g << 2, pixel_.r << 3);
    }

    ColorGrayscale ColorBGR565::ToGrayscale() const
    {
        return ColorGrayscale(((pixel_.r << 3) + (pixel_.g << 2) + (pixel_.b << 3)) / 3);
    }

    ColorBW ColorBGR565::ToBW() const
    {
        return ColorBW(pixel_.r > 15 || pixel_.g > 31 || pixel_.b > 15);
    }
}