#include "color.h"

namespace paint
{
    ColorRGB565 ColorRGB565::ToRGB565() const
    {
        return ColorRGB565(*this);
    }

    ColorRGB888 ColorRGB565::ToRGB888() const
    {
        return ColorRGB888(red_ << 3, green_ << 2, blue_ << 3);
    }

    ColorRGB565 ColorRGB888::ToRGB565() const
    {
        return ColorRGB565(red_ >> 3, green_ >> 2, blue_ >> 3);
    }

    ColorRGB888 ColorRGB888::ToRGB888() const
    {
        return ColorRGB888(*this);
    }

}