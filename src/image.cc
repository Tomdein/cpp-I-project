#include "image.h"

namespace paint
{
    void Image::Undo()
    {
        // If no image to return to -> nothing to do
        if (image_data_undo_history_.size() == 0)
            return;

        // Get the data from the undo list
        std::unique_ptr<DataPixels> new_data(std::move(image_data_undo_history_.front()));
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
    }

    void Image::Redo()
    {
        // If no image in redo list -> nothing to do
        if (image_data_redo_history_.size() == 0)
            return;

        // Get the data from the redo list
        std::unique_ptr<DataPixels> new_data(std::move(image_data_redo_history_.front()));
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
    }

    void Image::ImageEditCallback()
    {
        // Once the image is edited, throw away the redo history
        image_data_redo_history_.clear();

        image_data_undo_history_.emplace_back(std::make_unique<DataPixels>(*image_data_));

        if (image_data_undo_history_.size() > kImageHistorySize)
        {
            image_data_undo_history_.pop_front();
        }
    }
}