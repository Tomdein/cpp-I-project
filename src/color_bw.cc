#include "colors.h"

namespace paint
{

    ColorRGB565 ColorBW::ToRGB565() const
    {
        return ColorRGB565(bw_palette[pixel_.w].r, bw_palette[pixel_.w].g, bw_palette[pixel_.w].b);
    }

    ColorBGR565 ColorBW::ToBGR565() const
    {
        return ColorBGR565(bw_palette[pixel_.w].b, bw_palette[pixel_.w].g, bw_palette[pixel_.w].r);
    }

    ColorRGB888 ColorBW::ToRGB888() const
    {
        return ColorRGB888(bw_palette[pixel_.w].r, bw_palette[pixel_.w].g, bw_palette[pixel_.w].b);
    }

    ColorBGR888 ColorBW::ToBGR888() const
    {
        return ColorBGR888(bw_palette[pixel_.w].b, bw_palette[pixel_.w].g, bw_palette[pixel_.w].r);
    }

    ColorGrayscale ColorBW::ToGrayscale() const
    {
        return ColorGrayscale(pixel_.w * 255);
    }

    ColorBW ColorBW::ToBW() const
    {
        return *this;
    }

    const PixelBGRA8888 bw_palette[2] = {{0, 0, 0, 0}, {255, 255, 255, 0}};
}