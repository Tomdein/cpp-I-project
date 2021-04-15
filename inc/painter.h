#ifndef PAINT_INC_PAINTER_H_
#define PAINT_INC_PAINTER_H_

#include <memory>
#include <optional>

#include "unit.h"
#include "point.h"
#include "color.h"
#include "rotation.h"
#include "data_pixels.h"

namespace paint
{
    class Painter
    {
    public:
        Painter() = default;

        void AttachImageData(const std::weak_ptr<DataPixels> &image_data)
        {
            image_data_ = image_data;
        }
        void DetachImageData()
        {
            image_data_.reset();
        }

        void SetNextColor(std::shared_ptr<Color> &color) { next_command_color_->SetColor(*color); }

        // Move to img?
        // virtual void LoadImage() = 0;
        // virtual void SaveImage() = 0;
        // virtual void GenerateMetadata() = 0;

        virtual void DrawLine(const Point &start, const Point &end);
        virtual void DrawCircle(const Point &start, const Point &radius,
                                bool fill = false,
                                std::optional<std::shared_ptr<Color>> fill_color = std::nullopt,
                                std::optional<std::shared_ptr<Color>> border_color = std::nullopt,
                                Unit border_width = Unit(1));
        virtual void DrawBucket(const Point &point, std::optional<std::shared_ptr<Color>> fill_color = std::nullopt);

        // Crop, Resize and Rotate modifies the headers
        virtual void Crop(const Point &corner1, const Point &corner2);
        virtual void Resize(const Point &new_size);
        virtual void Rotate(Rotation rotation);

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        virtual void InvertColors();
        virtual void ConvertToGrayscale();

        // Undo and Redo *CAN* modify the headers
        virtual void Undo();
        virtual void Redo();

    private:
        std::unique_ptr<Color> next_command_color_;
        std::weak_ptr<DataPixels> image_data_;
    };
}

#endif // PAINT_INC_PAINTER_H_