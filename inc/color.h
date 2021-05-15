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
    class ColorBGR565;
    class ColorRGB888;
    class ColorBGR888;
    class ColorGrayscale;
    class ColorBW;

    class Color
    {
    public:
        Color() = default;
        virtual ~Color() = default;

        virtual Color *clone() const = 0;

        virtual ColorRGB565 ToRGB565() const = 0;
        virtual ColorBGR565 ToBGR565() const = 0;
        virtual ColorRGB888 ToRGB888() const = 0;
        virtual ColorBGR888 ToBGR888() const = 0;
        virtual ColorGrayscale ToGrayscale() const = 0;
        virtual ColorBW ToBW() const = 0;

        virtual void SetColor(const Color &other) = 0;
        virtual void InvertColor() = 0;
        virtual void Interpolate(const std::shared_ptr<Color> &c1, const std::shared_ptr<Color> &c2, float percent_c1) = 0;

        virtual void SetFromData(const void *data) = 0;
        virtual void *GetData() = 0;
        virtual size_t GetDataSize() const = 0;
        virtual size_t GetDataSizeBits() const = 0;
    };
}

#endif // PAINT_INC_COLOR_H_