#ifndef PAINT_INC_COLOR_RGB565_H_
#define PAINT_INC_COLOR_RGB565_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{
    class ColorRGB565 : public Color
    {
    public:
        ColorRGB565() = default;
        ColorRGB565(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB565() override {}
        virtual ColorRGB565 *clone() const override { return new ColorRGB565{*this}; }

        //ColorRGB565(const ColorRGB565 &other) : pixel_.red(other.pixel_.red), pixel_.green(other.pixel_.green), pixel_.blue(other.pixel_.blue) {}
        ColorRGB565(const ColorRGB565 &other) : pixel_{other.pixel_} {}
        ColorRGB565(const Color &other) { *this = std::move(other.ToRGB565()); }
        ColorRGB565(ColorRGB565 &&other) : pixel_(std::exchange(other.pixel_, PixelRGB565{0, 0, 0})) {}

        // ColorRGB565 &operator=(const Color &other) { return ColorRGB565(std::move(other.ToRGB565())); }
        ColorRGB565 &operator=(const ColorRGB565 &other) = default;
        bool operator==(const ColorRGB565 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorBGR565 ToBGR565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorBGR888 ToBGR888() const override;
        virtual ColorGrayscale ToGrayscale() const override;
        virtual ColorBW ToBW() const override;

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
        virtual size_t GetDataSizeBits() const override { return 16; };

    private:
        PixelRGB565 pixel_;
    };
}

#endif // PAINT_INC_COLOR_RGB565_H_