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
        return ColorGrayscale((pixel_.r + pixel_.g + pixel_.b) / 3);
    }

    ColorBW ColorRGB888::ToBW() const
    {
        return ColorBW(pixel_.r > 127 || pixel_.g > 127 || pixel_.b > 127);
    }
}