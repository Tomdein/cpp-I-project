#include "colors.h"

namespace paint
{
    ColorGrayscale::ColorGrayscale(uint8_t white) : pixel_{white} {}

    ColorRGB565 ColorGrayscale::ToRGB565() const
    {
        throw;
        return ColorRGB565();
    }

    ColorRGB888 ColorGrayscale::ToRGB888() const
    {
        throw;
        return ColorRGB888();
    }

    ColorGrayscale ColorGrayscale::ToGreyscale() const
    {
        return ColorGrayscale(*this);
    }
}