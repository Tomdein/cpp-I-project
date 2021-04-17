#ifndef PAINT_INC_COLOR_H_
#define PAINT_INC_COLOR_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"

namespace paint
{
    class ColorRGB565;
    class ColorRGB888;
    class ColorGrayscale;

    class Color
    {
    public:
        Color() = default;
        virtual ~Color() {}
        virtual Color *clone() const = 0;

        virtual ColorRGB565 ToRGB565() const = 0;
        virtual ColorRGB888 ToRGB888() const = 0;
        virtual ColorGrayscale ToGreyscale() const = 0;

        virtual void SetColor(const Color &other) = 0;
        virtual void InvertColor() = 0;
        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) = 0;

        virtual void SetFromData(const void *data) = 0;
        virtual void *GetData() = 0;
        virtual size_t GetDataSize() const = 0;
    };

    class ColorRGB565 : public Color
    {
    public:
        ColorRGB565() = default;
        ColorRGB565(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB565() {}
        virtual ColorRGB565 *clone() const override { return new ColorRGB565{*this}; }

        //ColorRGB565(const ColorRGB565 &other) : pixel_.red(other.pixel_.red), pixel_.green(other.pixel_.green), pixel_.blue(other.pixel_.blue) {}
        ColorRGB565(const ColorRGB565 &other) : pixel_{other.pixel_} {}
        ColorRGB565(const Color &other) { *this = std::move(other.ToRGB565()); }
        ColorRGB565(ColorRGB565 &&other) : pixel_(std::exchange(other.pixel_, PixelRGB565{0, 0, 0})) {}

        // ColorRGB565 &operator=(const Color &other) { return ColorRGB565(std::move(other.ToRGB565())); }
        ColorRGB565 &operator=(const ColorRGB565 &other) = default;
        bool operator==(const ColorRGB565 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorGrayscale ToGreyscale() const override;

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelRGB565 *>(data), 1, &pixel_);
            // memcpy(&pixel_, data, sizeof(PixelRGB565));
        };
        virtual void SetColor(const Color &other) { *this = std::move(other.ToRGB565()); };
        void SetColor(uint8_t red, uint8_t green, uint8_t blue)
        {
            pixel_.r = red;
            pixel_.g = green;
            pixel_.b = blue;
        }

        virtual void InvertColor() override
        {
            pixel_.r = ~pixel_.r;
            pixel_.g = ~pixel_.g;
            pixel_.b = ~pixel_.b;
        }

        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) override
        {
            ColorRGB565 c1_RGB565(c1->ToRGB565());
            ColorRGB565 c2_RGB565(c2->ToRGB565());

            // Interpolate the color
            pixel_.r = c1_RGB565.pixel_.r * percent_c1 + c2_RGB565.pixel_.r * (1 - percent_c1);
            pixel_.g = c1_RGB565.pixel_.g * percent_c1 + c2_RGB565.pixel_.g * (1 - percent_c1);
            pixel_.b = c1_RGB565.pixel_.b * percent_c1 + c2_RGB565.pixel_.b * (1 - percent_c1);
        };

        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelRGB565); };

    private:
        PixelRGB565 pixel_;
    };

    class ColorRGB888 : public Color
    {
    public:
        ColorRGB888() = default;
        ColorRGB888(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB888() {}
        virtual ColorRGB888 *clone() const override { return new ColorRGB888{*this}; }

        ColorRGB888(const ColorRGB888 &other) : pixel_{other.pixel_.r, other.pixel_.g, other.pixel_.b} {}
        ColorRGB888(const Color &other) { *this = std::move(other.ToRGB888()); }
        ColorRGB888(ColorRGB888 &&other) = default;

        ColorRGB888 &operator=(const ColorRGB888 &other) = default;
        bool operator==(const ColorRGB888 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorGrayscale ToGreyscale() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToRGB888()); };
        void SetColor(uint8_t red, uint8_t green, uint8_t blue)
        {
            pixel_.r = red;
            pixel_.g = green;
            pixel_.b = blue;
        }

        virtual void InvertColor() override
        {
            pixel_.r = ~pixel_.r;
            pixel_.g = ~pixel_.g;
            pixel_.b = ~pixel_.b;
        }

        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) override
        {
            ColorRGB888 c1_RGB888(c1->ToRGB888());
            ColorRGB888 c2_RGB888(c2->ToRGB888());

            // Interpolate the color
            pixel_.r = c1_RGB888.pixel_.r * percent_c1 + c2_RGB888.pixel_.r * (1 - percent_c1);
            pixel_.g = c1_RGB888.pixel_.g * percent_c1 + c2_RGB888.pixel_.g * (1 - percent_c1);
            pixel_.b = c1_RGB888.pixel_.b * percent_c1 + c2_RGB888.pixel_.b * (1 - percent_c1);
        };

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelRGB888 *>(data), 1, &pixel_);
            //memcpy(&pixel_, data, sizeof(PixelRGB888));
        };
        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelRGB888); };

    private:
        PixelRGB888 pixel_;
    };

    class ColorGrayscale : public Color
    {
    public:
        ColorGrayscale() = default;
        ColorGrayscale(uint8_t white) : pixel_{white} {}
        virtual ~ColorGrayscale() {}
        virtual ColorGrayscale *clone() const override { return new ColorGrayscale{*this}; }

        ColorGrayscale(const ColorGrayscale &other) : pixel_{other.pixel_.w} {}
        ColorGrayscale(const Color &other) { *this = std::move(other.ToRGB888()); }
        ColorGrayscale(ColorGrayscale &&other) = default;

        ColorGrayscale &operator=(const ColorGrayscale &other) = default;
        bool operator==(const ColorGrayscale &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorGrayscale ToGreyscale() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToRGB888()); };
        void SetColor(uint8_t white)
        {
            pixel_.w = white;
        }

        virtual void InvertColor() override
        {
            pixel_.w = ~pixel_.w;
        }

        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) override
        {
            ColorGrayscale c1_Greyscale(c1->ToRGB888());
            ColorGrayscale c2_Greyscale(c2->ToRGB888());

            // Interpolate the color
            pixel_.w = c1_Greyscale.pixel_.w * percent_c1 + c2_Greyscale.pixel_.w * (1 - percent_c1);
        };

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelGrayscale *>(data), 1, &pixel_);
        };
        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelGrayscale); };

    private:
        PixelGrayscale pixel_;
    };
}

#endif // PAINT_INC_COLOR_H_