#include "color.h"

namespace paint
{

    ColorRGB565::operator ColorRGB565() const
    {
        return *this;
    }

    ColorRGB565::operator ColorRGB888() const
    {
        return ColorRGB888(red_, green_, blue_);
    }

    ColorRGB888::operator ColorRGB565() const
    {
        return ColorRGB565(red_ >> 3, green_ >> 2, blue_ >> 3);
    }

    ColorRGB888::operator ColorRGB888() const
    {
        return *this;
    }
}