#ifndef PAINT_INC_IMAGE_DATA_H_
#define PAINT_INC_IMAGE_DATA_H_

#include <cstdint>

#include "color.h"
#include "point.h"

namespace paint
{

    class ImageData
    {
    public:
        void SetNextCommandColor(std::shared_ptr<paint::Color> &color) { next_command_color_ = color; };

        void DrawLine();
        void DrawCircle();
        void DrawBucket();

        void Crop();
        void Resize();
        void Rotate();

        void InvertColors();
        void ConvertToGrayscale();

        void Undo();
        void Redo();

    private:
        Point image_size_;
        const std::shared_ptr<Color> data_color_;
        std::shared_ptr<Color> next_command_color_;
    };
}

#endif // PAINT_INC_IMAGE_DATA_H_