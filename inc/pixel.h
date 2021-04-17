#ifndef PAINT_INC_PIXEL_H_
#define PAINT_INC_PIXEL_H_

#include <cstdint>

namespace paint
{
    /**
     * @brief A structure for storing 2 bytes wide RGB color.
     * 
     * This structure saves RGB color information in 2 bytes:
     *  5 bits for red color
     *  6 bits for green color
     *  5 bits for blue color
     * 
     */
    struct PixelRGB565
    {
        uint16_t r : 5; /// Red color
        uint16_t g : 6; /// Green color
        uint16_t b : 5; /// Blue color

        bool operator==(const PixelRGB565 &other) const
        {
            return r == other.r && g == other.g && b == other.b;
        }
    };

    /**
     * @brief A structure for storing 3 bytes wide RGB color.
     * 
     * This structure saves RGB color information in 3 bytes:
     *  8 bits for red color
     *  8 bits for green color
     *  8 bits for blue color
     * 
     */
    struct PixelRGB888
    {
        uint8_t r : 8; /// Red color
        uint8_t g : 8; /// Green color
        uint8_t b : 8; /// Blue color

        bool operator==(const PixelRGB888 &other) const
        {
            return r == other.r && g == other.g && b == other.b;
        }
    };

    /**
     * @brief A structure for storing 1 byte wide grayscale color.
     * 
     * This structure saves grayscale color information in 1 byte:
     *  8 bits for graycale color
     * 
     */
    struct PixelGrayscale
    {
        uint8_t w : 8; /// Grayscale color

        bool operator==(const PixelGrayscale &other) const
        {
            return w == other.w;
        }
    };
}

#endif // PAINT_INC_PIXEL_H_