#ifndef PAINT_INC_IMAGE_DATA_H_
#define PAINT_INC_IMAGE_DATA_H_

#include <cstdint>
#include <optional>

#include "unit.h"
#include "point.h"
#include "color.h"
#include "data_pixel.h"
#include "rotation.h"

namespace paint
{

    // Accepts any class 'Color'
    template <class T>
    class ImageData
    {
    public:
        ImageData(Point image_size) : image_size_(image_size)
        {
            data_pixels_ = std::make_unique<DataPixel>(image_size, sizeof(data_color_.data()));
            data_color_ = std::make_unique<T>();
        }

        // Duplicate image data structure
        ImageData<T> duplicate()
        {
            ImageData<T> id(image_size_);
            id.next_command_color_->SetColor(*next_command_color_);
            id->CopyData(*data_pixels_);
            return id;
        }

        // Copy the color to next_command_color_
        void SetNextCommandColor(std::shared_ptr<Color> &color) { next_command_color_.SetColor(*color); };

        void DrawLine(const Point &start, const Point &end);
        void DrawCircle(const Point &start, const Point &radius,
                        bool fill = false,
                        std::optional<T &> fill_color = std::nullopt,
                        std::optional<T &> border_color = std::nullopt,
                        Unit border_width = Unit(1));
        void DrawBucket(const Point &point, std::optional<T &> fill_color = std::nullopt);

        void Crop(const Point &corner1, const Point &corner2);
        void Resize(const Point &new_size);
        void Rotate(Rotation rotation);

        void InvertColors();
        void ConvertToGrayscale();

        void Undo();
        void Redo();

    private:
        Point image_size_;
        const std::unique_ptr<T> data_color_;
        std::unique_ptr<T> next_command_color_;
        std::unique_ptr<DataPixel> data_pixels_;
    };
}

#endif // PAINT_INC_IMAGE_DATA_H_