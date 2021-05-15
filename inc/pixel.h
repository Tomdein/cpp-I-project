#ifndef PAINT_INC_PIXEL_H_
#define PAINT_INC_PIXEL_H_

#include <cstdint>

namespace paint
{
    /**
     * @brief A structure for storing 4 bytes wide BGR color with alpha channel.
     * 
     * This structure saves BGR color information with alpha channel in 4 bytes:
     *  8 bits for blue color
     *  8 bits for green color
     *  8 bits for red color
     *  8 bits for alpha
     * 
     */
    struct PixelBGRA8888
    {
        uint8_t b : 8; /// Blue color
        uint8_t g : 8; /// Green color
        uint8_t r : 8; /// Red color
        uint8_t a : 8; /// Alpha color

        bool operator==(const PixelBGRA8888 &other) const
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
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
     * @brief A structure for storing 3 bytes wide BGR color.
     * 
     * This structure saves BGR color information in 3 bytes:
     *  8 bits for blue color
     *  8 bits for green color
     *  8 bits for red color
     * 
     */
    struct PixelBGR888
    {
        uint8_t b : 8; /// Blue color
        uint8_t g : 8; /// Green color
        uint8_t r : 8; /// Red color

        bool operator==(const PixelBGR888 &other) const
        {
            return r == other.r && g == other.g && b == other.b;
        }
    };

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
     * @brief A structure for storing 2 bytes wide BGR color.
     * 
     * This structure saves BGR color information in 2 bytes:
     *  5 bits for blue color
     *  6 bits for green color
     *  5 bits for red color
     * 
     */
    struct PixelBGR565
    {
        uint16_t b : 5; /// Blue color
        uint16_t g : 6; /// Green color
        uint16_t r : 5; /// Red color

        bool operator==(const PixelBGR565 &other) const
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

    /**
     * @brief A structure for storing 1 byte wide black and white color.
     * 
     * This structure saves black and white color information in 1 byte:
     *  1 bits for b & w color
     * 
     */
    struct PixelBW
    {
        uint8_t w : 1; /// Grayscale color

        bool operator==(const PixelBW &other) const
        {
            return w == other.w;
        }
    };
}

#endif // PAINT_INC_PIXEL_H_