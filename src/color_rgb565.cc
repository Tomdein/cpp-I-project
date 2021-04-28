#include "colors.h"

namespace paint
{
    ColorRGB565 ColorRGB565::ToRGB565() const
    {
        return ColorRGB565(*this);
    }

    ColorRGB888 ColorRGB565::ToRGB888() const
    {
        return ColorRGB888(pixel_.r << 3, pixel_.g << 2, pixel_.b << 3);
    }

    ColorGrayscale ColorRGB565::ToGrayscale() const
    {
        return ColorGrayscale(((pixel_.r << 3) + (pixel_.g << 2) + (pixel_.b << 3)) / 3);
    }

    ColorBW ColorRGB565::ToBW() const
    {
        return ColorBW(pixel_.r > 15 || pixel_.g > 31 || pixel_.b > 15);
    }
}