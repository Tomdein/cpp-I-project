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
    * @brief Abtract class for image
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
        Image() : painter(std::bind(&Image::ImageEditCallback, this)) {}
        virtual ~Image() {}

        Painter painter; /// A painter used to edit the image.

        /**
         * @brief Moves back in history of edited image versions.
         * 
         * Stores the Image::image_data_ to the Image::image_data_redo_history_ and loads previous image from Image::image_data_undo_history_.
         * If there is no image remaining in the Image::image_data_undo_history_, this function does nothing.
         * 
         */
        void Undo();
        /**
         * @brief Moves redoes the previously undone image changes.
         * 
         * Stores the Image::image_data_ to the Image::image_data_undo_history_ and loads previously undone image from Image::image_data_redo_history_.
         * If there is no image remaining in the Image::image_data_redo_history_, this function does nothing.
         * 
         */
        void Redo();

        virtual void CreateImage() = 0;
        virtual void LoadImage() = 0;
        virtual void SaveImage() = 0;

        /**
         * @brief Sets the output image path
         * 
         */
        void SetOutputImage(const File &file_out) { file_out_ = file_out; }

        /**
         * @brief This method imforms Image that the data has been edited.
         * 
         * When the Image::painter edits the image data, it calls this callback so that Image knows the Image::image_data_ has been edited.
         * 
         */
        void ImageEditCallback();

    protected:
        File file_in_;
        File file_out_;
        std::shared_ptr<DataPixels> image_data_;                          /// Stores the current image data.
        std::deque<std::shared_ptr<DataPixels>> image_data_undo_history_; /// Stores the image history.
        std::deque<std::shared_ptr<DataPixels>> image_data_redo_history_; /// Stores the previously undone images.
        std::unique_ptr<uint8_t[]> unprocessed_data;                      /// Storage for data before saving/loading pixel data from/to file.

        // TODO: get rid of has_fixed_size_ & has_image_
        bool has_fixed_size_ = false;
        bool has_image_ = false;

        /**
         * @brief Constant that sets thge size of the image history buffers.
         * 
         */
        enum
        {
            kImageHistorySize = 5000,
        };

    private:
        virtual void CreateDataBuffer() = 0;
        virtual void GenerateMetadata() = 0;
    };
}
#endif // PAINT_INC_IMAGE_H_