#include "image.h"

namespace paint
{
    void Image::Undo()
    {
        // If no image to return to -> nothing to do
        if (image_data_undo_history_.size() == 0)
            return;

        // Get the data from the undo list
        std::shared_ptr<DataPixels> new_data(std::move(image_data_undo_history_.front()));
        image_data_undo_history_.pop_front();

        // Place current data to the redo list
        image_data_redo_history_.emplace_back(std::move(image_data_));

        // If image_data_redo_history is full -> remove last image
        if (image_data_redo_history_.size() > kImageHistorySize)
        {
            image_data_redo_history_.pop_front();
        }

        // Set the image from undo list as current
        image_data_.swap(new_data);

        // Attach painter to the data
        painter.AttachImageData(image_data_);
    }

    void Image::Redo()
    {
        // If no image in redo list -> nothing to do
        if (image_data_redo_history_.size() == 0)
            return;

        // Get the data from the redo list
        std::shared_ptr<DataPixels> new_data(std::move(image_data_redo_history_.front()));
        image_data_redo_history_.pop_front();

        // Place current data to the undo list
        image_data_undo_history_.emplace_back(std::move(image_data_));

        // If image_data_undo_history is full -> remove last image
        if (image_data_undo_history_.size() > kImageHistorySize)
        {
            image_data_undo_history_.pop_front();
        }

        // Set the image from redo list as current
        image_data_.swap(new_data);

        // Attach painter to the data
        painter.AttachImageData(image_data_);
    }

    void Image::ImageEditCallback()
    {
        // Once the image is edited, throw away the redo history
        image_data_redo_history_.clear();

        // Duplicate the data
        std::shared_ptr<DataPixels> data_copy = std::make_shared<DataPixels>(*image_data_);

        // Save the duplicated data into history -> image_data_ is ready to be drawn on
        image_data_undo_history_.emplace_back(std::move(data_copy));

        if (image_data_undo_history_.size() > kImageHistorySize)
        {
            image_data_undo_history_.pop_front();
        }
    }
}