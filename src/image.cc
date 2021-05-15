#include "image.h"

#include <iostream>

namespace paint
{
    void Image::DumpImageHistory()
    {
        // Clear image_dump dir
        std::filesystem::remove_all("./image_dump");

        // Create image_dump dir
        if (!std::filesystem::create_directory("./image_dump"))
        {
            std::cerr << "Could not create image_dump directory!" << std::endl;
            return;
        }

        int idx = 0;
        auto save_data = image_data_;

        // Go through every picture in undo history and save it
        for (auto &img_data : image_data_undo_history_)
        {
            // Set file name and filepath
            std::stringstream s;
            s.str(std::string());
            s << "./image_dump/img_" << idx << ".bmp";

            // Set data
            image_data_ = img_data;

            //TODO: support dumping in different formats.
            // file_out_ = File{FileType::kBMP, s.str()};
            // SaveImage(file_out_);

            // Save the image
            SaveImage(s.str());

            idx++;
        }

        image_data_ = save_data;
    }

    void Image::Undo()
    {
        // If no image to return to -> nothing to do
        if (image_data_undo_history_.size() == 0)
            return;

        // Get the last changed data, if undo was not the last command -> Get discard unbuffered edits.
        std::shared_ptr<DataPixels> last_change;
        if (!undo_was_last_command_)
        {
            last_change = image_data_undo_history_.back();
            image_data_undo_history_.pop_back();
        }
        else
        {
            last_change = image_data_;
        }

        // Get the data from the undo list
        std::shared_ptr<DataPixels> new_data(std::move(image_data_undo_history_.back()));
        image_data_undo_history_.pop_back();

        // Place last change to the redo list
        image_data_redo_history_.emplace_back(std::move(last_change));

        // If image_data_redo_history is full -> remove last image
        if (image_data_redo_history_.size() > kImageHistorySize)
            image_data_redo_history_.pop_front();

        // Set the image from undo list as current
        image_data_.swap(new_data);

        // Attach painter to the data
        painter.AttachImageData(image_data_);

        undo_was_last_command_ = true;
    }

    void Image::Redo()
    {
        // If no image in redo list -> nothing to do
        if (image_data_redo_history_.size() == 0)
            return;

        // Get the data from the redo list
        std::shared_ptr<DataPixels> new_data(std::move(image_data_redo_history_.back()));
        image_data_redo_history_.pop_back();

        // Place current data to the undo list
        image_data_undo_history_.emplace_back(std::move(image_data_));

        // If image_data_undo_history is full -> remove last image
        if (image_data_undo_history_.size() > kImageHistorySize)
            image_data_undo_history_.pop_front();

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

        // Undo buffer is full -> remove front image (the oldest image)
        if (image_data_undo_history_.size() > kImageHistorySize)
            image_data_undo_history_.pop_front();

        undo_was_last_command_ = false;
    }
}