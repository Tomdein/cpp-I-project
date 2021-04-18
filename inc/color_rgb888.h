#ifndef PAINT_INC_COLOR_RGB888_H_
#define PAINT_INC_COLOR_RGB888_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{
    class ColorRGB888 : public Color
    {
    public:
        ColorRGB888() = default;
        ColorRGB888(uint8_t red, uint8_t green, uint8_t blue) : pixel_{red, green, blue} {}
        virtual ~ColorRGB888() override{};
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
}

#endif // PAINT_INC_COLOR_RGB888_H_