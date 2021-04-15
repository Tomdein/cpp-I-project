#ifndef PAINT_INC_IMAGE_H_
#define PAINT_INC_IMAGE_H_

#include <deque>
#include <memory>

#include "file.h"
#include "data_pixels.h"
#include "painter.h"

namespace paint
{

    class Image
    {
    public:
        virtual void LoadImage() = 0;
        virtual void SaveImage() = 0;
        virtual void GenerateMetadata() = 0;

    protected:
        File file_;

    private:
        std::unique_ptr<DataPixels> image_data_;
        std::deque<std::unique_ptr<DataPixels>> image_data_history_;
        Painter painter_;
    };
}
#endif // PAINT_INC_IMAGE_H_