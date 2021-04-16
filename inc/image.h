#ifndef PAINT_INC_IMAGE_H_
#define PAINT_INC_IMAGE_H_

#include <deque>
#include <memory>
#include <functional>

#include "file.h"
#include "data_pixels.h"
#include "painter.h"

namespace paint
{
    /**
    * @brief Abtract class holding image data
    * 
    * Class Image provides interface for creating/loading/storing image data.
    * 
    * Image has a Image::painter that is used for drawing on image.
    * 
    * It also provides Image::Undo() & Image::Redo() that implement history of edits.
    * If Image::Undo() is called and then Image::painter draws on image, the call to Image::Redo() does nothing. 
    * 
    * Contains current image pixel data in Image::image_data_ and also has buffered previous changes to pixel data in Image::image_data_history.
    */
    class Image
    {
    public:
        Image() : painter(callback_function_) {}

        Painter painter;

        void Redo(){};
        void Undo(){};

        virtual void CreateImage() = 0;
        virtual void LoadImage() = 0;
        virtual void SaveImage() = 0;

        void ImageEditCallback();

    protected:
        File file_;
        std::unique_ptr<DataPixels> image_data_;
        std::deque<std::unique_ptr<DataPixels>> image_data_history_;

        bool has_fixed_size_ = false;
        bool has_image_ = false;

        virtual void CreateDataBuffer() = 0;
        virtual void GenerateMetadata() = 0;

    private:
        std::function<void()> callback_function_ = std::bind(&Image::ImageEditCallback, this);
    };
}
#endif // PAINT_INC_IMAGE_H_