#ifndef PAINT_INC_IMAGE_TIFF_H_
#define PAINT_INC_IMAGE_TIFF_H_

#include "image.h"

namespace paint
{

    class ImageTIFF : public Image
    {
    public:
        ImageTIFF() = default;

        virtual void LoadImage() override {}
        virtual void SaveImage() override {}
        virtual void GenerateMetadata() override {}

    private:
    };
}

#endif // PAINT_INC_IMAGE_TIFF_H_