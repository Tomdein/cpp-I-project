#ifndef PAINT_INC_PAINTER_H_
#define PAINT_INC_PAINTER_H_

#include <memory>
#include <optional>

#include "unit.h"
#include "point.h"
#include "color.h"
#include "rotation.h"

namespace paint
{
    class Painter
    {
    public:
        Painter() = default;

        virtual void SetData();

        void SetNextColor(std::shared_ptr<Color> &color) { next_command_color_->SetColor(*color); }

        // Move to img?
        // virtual void LoadImage() = 0;
        // virtual void SaveImage() = 0;
        // virtual void GenerateMetadata() = 0;

        virtual void DrawLine(const Point &start, const Point &end) = 0;
        virtual void DrawCircle(const Point &start, const Point &radius,
                                bool fill = false,
                                std::optional<Color &> fill_color = std::nullopt,
                                std::optional<Color &> border_color = std::nullopt,
                                Unit border_width = Unit(1)) = 0;
        virtual void DrawBucket(const Point &point, std::optional<Color &> fill_color = std::nullopt) = 0;

        // Crop, Resize and Rotate modifies the headers
        virtual void Crop(const Point &corner1, const Point &corner2) = 0;
        virtual void Resize(const Point &new_size) = 0;
        virtual void Rotate(Rotation rotation) = 0;

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        virtual void InvertColors() = 0;
        virtual void ConvertToGrayscale() = 0;

        // Undo and Redo *CAN* modify the headers
        virtual void Undo() = 0;
        virtual void Redo() = 0;

    private:
        std::unique_ptr<Color> next_command_color_;
    };
}

#endif // PAINT_INC_PAINTER_H_