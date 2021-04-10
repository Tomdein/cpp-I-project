#ifndef PAINT_INC_COLOR_H_
#define PAINT_INC_COLOR_H_

#include <utility>
#include <cstdint>

#include "pixel.h"

namespace paint
{
    class ColorRGB565;
    class ColorRGB888;

    class Color
    {
    public:
        Color() = default;
        virtual ~Color() {}

        virtual ColorRGB565 ToRGB565() const = 0;
        virtual ColorRGB888 ToRGB888() const = 0;

        virtual void SetColor(const Color &other) = 0;
    };

    class ColorRGB565 : public Color
    {
    public:
        ColorRGB565() = default;
        ColorRGB565(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB565() {}

        //ColorRGB565(const ColorRGB565 &other) : pixel_.red(other.pixel_.red), pixel_.green(other.pixel_.green), pixel_.blue(other.pixel_.blue) {}
        ColorRGB565(const ColorRGB565 &other) : pixel_{other.pixel_} {}
        ColorRGB565(const Color &other) { *this = std::move(other.ToRGB565()); }
        ColorRGB565(ColorRGB565 &&other) : pixel_(std::exchange(other.pixel_, PixelRGB565{0, 0, 0})) {}

        // ColorRGB565 &operator=(const Color &other) { return ColorRGB565(std::move(other.ToRGB565())); }
        ColorRGB565 &operator=(const ColorRGB565 &other) = default;
        bool operator==(const ColorRGB565 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToRGB565()); };
        void SetColor(uint8_t red, uint8_t green, uint8_t blue)
        {
            pixel_.r = red;
            pixel_.g = green;
            pixel_.b = blue;
        }

        PixelRGB565 GetData() { return pixel_; }

    private:
        PixelRGB565 pixel_;
    };

    class ColorRGB888 : public Color
    {
    public:
        ColorRGB888() = default;
        ColorRGB888(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB888() {}

        ColorRGB888(const ColorRGB888 &other) : pixel_{other.pixel_.r, other.pixel_.g, other.pixel_.b} {}
        ColorRGB888(const Color &other) { *this = std::move(other.ToRGB888()); }
        ColorRGB888(ColorRGB888 &&other) = default;

        ColorRGB888 &operator=(const ColorRGB888 &other) = default;
        bool operator==(const ColorRGB888 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToRGB888()); };
        void SetColor(uint8_t red, uint8_t green, uint8_t blue)
        {
            pixel_.r = red;
            pixel_.g = green;
            pixel_.b = blue;
        }

        PixelRGB888 GetData() { return pixel_; }

    private:
        PixelRGB888 pixel_;
    };
}

#endif // PAINT_INC_COLOR_H_