#ifndef PAINT_INC_IMAGE_TIFF_H_
#define PAINT_INC_IMAGE_TIFF_H_

#include "image.h"

namespace paint
{

    template <class T>
    class ImageTIFF : Image<T>
    {
    public:
        ImageTIFF() = default;

        virtual void LoadImage() override {}
        virtual void SaveImage() override {}
        virtual void GenerateMetadata() override {}

    private:
        HeaderBMP header_bmp_;
        HeaderBMPInfo header_bmp_info_;
    };
}

#endif // PAINT_INC_IMAGE_TIFF_H_