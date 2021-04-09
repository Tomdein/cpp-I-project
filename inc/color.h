#ifndef PAINT_INC_COLOR_H_
#define PAINT_INC_COLOR_H_

#include <utility>
#include <cstdint>

namespace paint
{
    class ColorRGB565;
    class ColorRGB888;

    class Color
    {
    public:
        Color() = default;
        virtual ~Color() {}

        virtual explicit operator ColorRGB565() const = 0;
        virtual explicit operator ColorRGB888() const = 0;

        virtual void SetColor(const Color &other) = 0;
    };

    class ColorRGB565 : public Color
    {
    public:
        ColorRGB565(uint8_t red, uint8_t green, uint8_t blue)
        {
            red_ = red;
            green_ = green;
            blue_ = blue;
        }
        virtual ~ColorRGB565() {}

        ColorRGB565(const ColorRGB565 &other) : red_(other.red_), green_(other.green_), blue_(other.blue_) {}
        ColorRGB565(ColorRGB565 &&other) : red_(std::exchange(other.red_, 0)), green_(std::exchange(other.green_, 0)), blue_(std::exchange(other.blue_, 0)) {}

        ColorRGB565 &operator=(ColorRGB565 other)
        {
            std::swap(red_, other.red_);
            std::swap(green_, other.green_);
            std::swap(blue_, other.blue_);
            return *this;
        }

        virtual explicit operator ColorRGB565() const override;
        virtual explicit operator ColorRGB888() const override;

        virtual void SetColor(const Color &other) { *this = static_cast<ColorRGB565>(other); };

    private:
        uint8_t red_;
        uint8_t green_;
        uint8_t blue_;
    };

    class ColorRGB888 : public Color
    {
    public:
        ColorRGB888(uint8_t red, uint8_t green, uint8_t blue)
        {
            red_ = red;
            green_ = green;
            blue_ = blue;
        }
        virtual ~ColorRGB888() {}

        ColorRGB888(const ColorRGB888 &other) : red_(other.red_), green_(other.green_), blue_(other.blue_) {}
        ColorRGB888(ColorRGB888 &&other) : red_(std::exchange(other.red_, 0)), green_(std::exchange(other.green_, 0)), blue_(std::exchange(other.blue_, 0)) {}

        ColorRGB888 &operator=(ColorRGB888 other)
        {
            std::swap(red_, other.red_);
            std::swap(green_, other.green_);
            std::swap(blue_, other.blue_);
            return *this;
        }

        virtual explicit operator ColorRGB565() const override;
        virtual explicit operator ColorRGB888() const override;

        virtual void SetColor(const Color &other) { *this = static_cast<ColorRGB888>(other); };

    private:
        uint8_t red_;
        uint8_t green_;
        uint8_t blue_;
    };
}

#endif // PAINT_INC_COLOR_H_