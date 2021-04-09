#ifndef PAINT_INC_PIXEL_H_
#define PAINT_INC_PIXEL_H_

#include <cstdint>

#include <color.h>

namespace paint
{
    struct PixelRGB565
    {
        uint16_t r : 5;
        uint16_t g : 6;
        uint16_t b : 5;
    };

    struct PixelRGB888
    {
        uint8_t r : 8;
        uint8_t g : 8;
        uint8_t b : 8;

        PixelRGB888()
    };
}

#endif // PAINT_INC_PIXEL_H_