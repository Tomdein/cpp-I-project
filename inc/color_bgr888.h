#ifndef PAINT_INC_COLOR_BGR888_H_
#define PAINT_INC_COLOR_BGR888_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{
    class ColorBGR888 : public Color
    {
    public:
        ColorBGR888() = default;
        ColorBGR888(uint8_t blue, uint8_t green, uint8_t red) : pixel_{blue, green, red} {}
        virtual ~ColorBGR888() override{};
        virtual ColorBGR888 *clone() const override { return new ColorBGR888{*this}; }

        ColorBGR888(const ColorBGR888 &other) : pixel_{other.pixel_.b, other.pixel_.g, other.pixel_.r} {}
        ColorBGR888(const Color &other) { *this = std::move(other.ToBGR888()); }
        ColorBGR888(ColorBGR888 &&other) = default;

        ColorBGR888 &operator=(const ColorBGR888 &other) = default;
        bool operator==(const ColorBGR888 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorBGR565 ToBGR565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorBGR888 ToBGR888() const override;
        virtual ColorGrayscale ToGrayscale() const override;
        virtual ColorBW ToBW() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToBGR888()); };
        void SetColor(uint8_t blue, uint8_t green, uint8_t red)
        {
            pixel_.b = blue;
            pixel_.g = green;
            pixel_.r = red;
        }

        virtual void InvertColor() override
        {
            pixel_.b = ~pixel_.b;
            pixel_.g = ~pixel_.g;
            pixel_.r = ~pixel_.r;
        }

        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) override
        {
            ColorBGR888 c1_BGR888(c1->ToBGR888());
            ColorBGR888 c2_BGR888(c2->ToBGR888());

            // Interpolate the color
            pixel_.b = c1_BGR888.pixel_.b * percent_c1 + c2_BGR888.pixel_.b * (1 - percent_c1);
            pixel_.g = c1_BGR888.pixel_.g * percent_c1 + c2_BGR888.pixel_.g * (1 - percent_c1);
            pixel_.r = c1_BGR888.pixel_.r * percent_c1 + c2_BGR888.pixel_.r * (1 - percent_c1);
        };

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelBGR888 *>(data), 1, &pixel_);
        };
        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelBGR888); };
        virtual size_t GetDataSizeBits() const override { return 24; };

    private:
        PixelBGR888 pixel_;
    };
}

#endif // PAINT_INC_COLOR_BGR888_H_