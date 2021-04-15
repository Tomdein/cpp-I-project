#ifndef PAINT_INC_IMAGE_H_
#define PAINT_INC_IMAGE_H_

#include <deque>
#include <memory>

#include "file.h"
#include "data_pixels.h"
#include "painter.h"

namespace paint
{

    class AbstractImage
    {
    public:
        virtual void SetNextColor(std::shared_ptr<Color> &color) = 0;

        virtual bool LoadImage() = 0;
        virtual bool SaveImage() = 0;
        virtual bool GenerateMetadata() = 0;

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

    protected:
        File file_;

    private:
        std::unique_ptr<DataPixels> image_data_;
        std::deque<std::unique_ptr<DataPixels>> image_data_history_;
        Painter painter_;
    };
}

#endif // PAINT_INC_IMAGE_H_