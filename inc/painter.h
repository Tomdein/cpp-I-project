#ifndef PAINT_INC_PAINTER_H_
#define PAINT_INC_PAINTER_H_

#include <memory>

#include "image_data.h"

namespace paint
{
    template <class T>
    class Painter
    {
    public:
        Painter() = default;

        void SetNextColor(std::shared_ptr<Color> &color) = 0;

        // Move to img?
        // virtual void LoadImage() = 0;
        // virtual void SaveImage() = 0;
        // virtual void GenerateMetadata() = 0;

        void DrawLine();
        void DrawCircle();
        void DrawBucket();

        // Crop, Resize and Rotate modifies the headers
        void Crop();
        void Resize();
        void Rotate();

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        void InvertColors();
        void ConvertToGrayscale();

        // Undo and Redo *CAN* modify the headers
        void Undo();
        void Redo();

        void SetNextColor(std::weak_ptr<Color> &color) final { image_data_.get()->SetNextCommandColor(color); }
    };
}

#endif // PAINT_INC_PAINTER_H_