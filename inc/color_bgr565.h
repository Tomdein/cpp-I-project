#ifndef PAINT_INC_COLOR_RBGR65_H_
#define PAINT_INC_COLOR_RBGR65_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{
    class ColorBGR565 : public Color
    {
    public:
        ColorBGR565() = default;
        ColorBGR565(uint8_t blue, uint8_t green, uint8_t red) : pixel_{blue, green, red} {}
        virtual ~ColorBGR565() override {}
        virtual ColorBGR565 *clone() const override { return new ColorBGR565{*this}; }

        //ColorBGR565(const ColorBGR565 &other) : pixel_.red(other.pixel_.red), pixel_.green(other.pixel_.green), pixel_.blue(other.pixel_.blue) {}
        ColorBGR565(const ColorBGR565 &other) : pixel_{other.pixel_} {}
        ColorBGR565(const Color &other) { *this = std::move(other.ToBGR565()); }
        ColorBGR565(ColorBGR565 &&other) : pixel_(std::exchange(other.pixel_, PixelBGR565{0, 0, 0})) {}

        // ColorBGR565 &operator=(const Color &other) { return ColorBGR565(std::move(other.ToBGR565())); }
        ColorBGR565 &operator=(const ColorBGR565 &other) = default;
        bool operator==(const ColorBGR565 &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorBGR565 ToBGR565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorBGR888 ToBGR888() const override;
        virtual ColorGrayscale ToGrayscale() const override;
        virtual ColorBW ToBW() const override;

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelBGR565 *>(data), 1, &pixel_);
        };
        virtual void SetColor(const Color &other) { *this = std::move(other.ToBGR565()); };
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
            ColorBGR565 c1_RGB565(c1->ToBGR565());
            ColorBGR565 c2_RGB565(c2->ToBGR565());

            // Interpolate the color
            pixel_.b = c1_RGB565.pixel_.b * percent_c1 + c2_RGB565.pixel_.b * (1 - percent_c1);
            pixel_.g = c1_RGB565.pixel_.g * percent_c1 + c2_RGB565.pixel_.g * (1 - percent_c1);
            pixel_.r = c1_RGB565.pixel_.r * percent_c1 + c2_RGB565.pixel_.r * (1 - percent_c1);
        };

        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelBGR565); };
        virtual size_t GetDataSizeBits() const override { return 16; };

    private:
        PixelBGR565 pixel_;
    };
}

#endif // PAINT_INC_COLOR_BGR565_H_