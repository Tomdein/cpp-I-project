#ifndef PAINT_INC_COLOR_GRAYSCALE_H_
#define PAINT_INC_COLOR_GRAYSCALE_H_

#include <utility>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>

#include "pixel.h"
#include "color.h"

namespace paint
{

    extern const PixelRGB888 grayscale_palette[256];

    class ColorGrayscale : public Color
    {
    public:
        ColorGrayscale() = default;
        ColorGrayscale(uint8_t white) : pixel_{white} {}
        virtual ~ColorGrayscale() override {}
        virtual ColorGrayscale *clone() const override { return new ColorGrayscale{*this}; }

        ColorGrayscale(const ColorGrayscale &other) : pixel_{other.pixel_.w} {}
        ColorGrayscale(const Color &other) { *this = std::move(other.ToGrayscale()); }
        ColorGrayscale(ColorGrayscale &&other) = default;

        ColorGrayscale &operator=(const ColorGrayscale &other)
        {
            pixel_ = other.pixel_;
            return *this;
        }
        bool operator==(const ColorGrayscale &other) const { return pixel_ == other.pixel_; }

        virtual ColorRGB565 ToRGB565() const override;
        virtual ColorRGB888 ToRGB888() const override;
        virtual ColorGrayscale ToGrayscale() const override;
        virtual ColorBW ToBW() const override;

        virtual void SetColor(const Color &other) { *this = std::move(other.ToGrayscale()); };
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
            ColorGrayscale c1_Grayscale(c1->ToGrayscale());
            ColorGrayscale c2_Grayscale(c2->ToGrayscale());

            // Interpolate the color
            pixel_.w = c1_Grayscale.pixel_.w * percent_c1 + c2_Grayscale.pixel_.w * (1 - percent_c1);
        };

        virtual void SetFromData(const void *data) override
        {
            std::copy_n(reinterpret_cast<const PixelGrayscale *>(data), 1, &pixel_);
        };
        virtual void *GetData() override { return reinterpret_cast<void *>(&pixel_); };
        virtual size_t GetDataSize() const override { return sizeof(PixelGrayscale); };

    private:
        PixelGrayscale pixel_;
        const PixelRGB888 (&grayscale_palette_)[256] = grayscale_palette;
    };
}

#endif // PAINT_INC_COLOR_GRAYSCALE_H_
