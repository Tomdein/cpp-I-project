#ifndef PAINT_INC_PAINTER_H_
#define PAINT_INC_PAINTER_H_

#include <memory>

#include "color.h"
#include "image_data.h"

namespace paint
{
    class Painter
    {
    public:
        Painter() = default;

        virtual void SetData();

        virtual void SetNextColor(std::shared_ptr<Color> &color) = 0;

        // Move to img?
        // virtual void LoadImage() = 0;
        // virtual void SaveImage() = 0;
        // virtual void GenerateMetadata() = 0;

        virtual void DrawLine() = 0;
        virtual void DrawCircle() = 0;
        virtual void DrawBucket() = 0;

        // Crop, Resize and Rotate modifies the headers
        virtual void Crop() = 0;
        virtual void Resize() = 0;
        virtual void Rotate() = 0;

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        virtual void InvertColors() = 0;
        virtual void ConvertToGrayscale() = 0;

        // Undo and Redo *CAN* modify the headers
        virtual void Undo() = 0;
        virtual void Redo() = 0;

        // void SetNextColor(std::weak_ptr<Color> &color) final { image_data_.get()->SetNextCommandColor(color); }

    private:
        std::unique_ptr<Color> color_format_;
    };
}

#endif // PAINT_INC_PAINTER_H_