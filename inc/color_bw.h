#ifndef PAINT_INC_COLOR_BW_H_
#define PAINT_INC_COLOR_BW_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{

    extern const PixelBGRA8888 bw_palette[2];

    class ColorBW : public Color
    {
    public:
        ColorBW() = default;
        ColorBW(uint8_t white) : pixel_{white} {}
        virtual ~ColorBW() override {}
        virtual ColorBW *clone() const override { return new ColorBW{*this}; }

        ColorBW(const ColorBW &other) : pixel_{other.pixel_.w} {}
        ColorBW(const Color &other) { *this = std::move(other.ToBW()); }
        ColorBW(ColorBW &&other) = default;

        ColorBW &operator=(const ColorBW &other)
        {
            pixel_ = other.pixel_;
            return *this;
        }
        bool operator==(const ColorBW &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorGrayscale ToGrayscale() const override;
        virtual ColorBW ToBW() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToBW()); }
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
            ColorBW c1_Grayscale(c1->ToBW());
            ColorBW c2_Grayscale(c2->ToBW());

            // Interpolate the color
            pixel_.w = c1_Grayscale.pixel_.w * percent_c1 + c2_Grayscale.pixel_.w * (1 - percent_c1);
        };

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelBW *>(data), 1, &pixel_);
        }
        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); }
        virtual size_t GetDataSize() const override { return sizeof(PixelBW); }
        virtual size_t GetDataSizeBits() const override { return 1; };

    private:
        PixelBW pixel_;
        const PixelBGRA8888 (&bw_palette_)[2] = bw_palette;
    };
}

#endif // PAINT_INC_COLOR_BW_H_