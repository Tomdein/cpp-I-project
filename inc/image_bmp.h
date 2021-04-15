#ifndef PAINT_INC_IMAGE_BPM_H_
#define PAINT_INC_IMAGE_BPM_H_

#include <cstdint>

#include "image.h"

namespace paint
{
    struct HeaderBMP
    {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };

    struct HeaderBMPInfo
    {
        uint32_t biSize;
        uint32_t biWidht;
        uint32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

    template <class T>
    class ImageBMP : Image<T>
    {
    public:
        virtual void LoadImage() override {}
        virtual void SaveImage() override {}
        virtual void GenerateMetadata() override {}

    private:
        HeaderBMP header_bmp_;
        HeaderBMPInfo header_bmp_info_;
    };

}

#endif // PAINT_INC_IMAGE_BPM_H_