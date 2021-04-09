#ifndef PAINT_INC_IMAGE_H_
#define PAINT_INC_IMAGE_H_

#include <deque>
#include <memory>

#include "file.h"
#include "image_data.h"

namespace paint
{

    class AbstractImage
    {
    public:
        void SetColor(std::shared_ptr<paint::Color> &color) { image_data_.get()->SetNextCommandColor(color); };

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

    private:
        File file_;
        std::shared_ptr<ImageData> image_data_;
        std::deque<std::shared_ptr<ImageData>> image_data_history_;
    };
}

#endif // PAINT_INC_IMAGE_H_