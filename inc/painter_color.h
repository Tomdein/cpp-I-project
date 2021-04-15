#ifndef PAINT_INC_PAINTER_COLOR_H_
#define PAINT_INC_PAINTER_COLOR_H_

#include <memory>

#include "image_data.h"

namespace paint
{
    template <class T>
    class PainterColor : public Painter
    {
    public:
        PainterColor() = default;

        virtual void SetNextColor(std::shared_ptr<Color> &color) = 0;

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

    private:
        std::weak_ptr<ImageData<T>> color_format_;
    };
}

#endif // PAINT_INC_PAINTER_COLOR_H_