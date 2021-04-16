#ifndef PAINT_INC_PAINTER_H_
#define PAINT_INC_PAINTER_H_

#include <memory>
#include <optional>
#include <functional>

#include "unit.h"
#include "point.h"
#include "color.h"
#include "rotation.h"
#include "data_pixels.h"

namespace paint
{
    class Image;

    class Painter
    {
    public:
        Painter(std::function<void()> image_edit_callback) : image_edit_callback_{std::move(image_edit_callback)} {}

        void AttachImageData(const std::weak_ptr<DataPixels> &image_data)
        {
            image_data_ = image_data;
        }
        void DetachImageData()
        {
            image_data_.reset();
        }

        void SetNextColor(std::shared_ptr<Color> &color);

        virtual void DrawLine(const BasePoint &start,
                              const BasePoint &end,
                              const std::optional<std::shared_ptr<Color>> &line_color_ = std::nullopt,
                              const std::optional<Unit> &line_width_ = std::nullopt);
        virtual void DrawCircle(const BasePoint &center,
                                const Unit &radius,
                                bool fill = false,
                                const std::optional<std::shared_ptr<Color>> &fill_color = std::nullopt,
                                const std::optional<std::shared_ptr<Color>> &border_color = std::nullopt,
                                const std::optional<Unit> &border_width = std::nullopt);
        virtual void DrawBucket(const BasePoint &point, const std::optional<std::shared_ptr<Color>> &fill_color = std::nullopt);

        // Crop, Resize and Rotate modifies the headers
        virtual void Crop(const BasePoint &corner1, const BasePoint &corner2);
        virtual void Resize(const BasePoint &new_size);
        virtual void Rotate(Rotation rotation);

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        virtual void InvertColors();
        virtual void ConvertToGrayscale();

    private:
        std::shared_ptr<Color> next_command_color_;
        std::weak_ptr<DataPixels> image_data_;

        std::function<void()> image_edit_callback_;
    };
}

#endif // PAINT_INC_PAINTER_H_