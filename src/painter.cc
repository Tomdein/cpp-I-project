#include "painter.h"
#include "unit.h"
#include "vec.h"

#include <vector>
#include <cstring>

namespace paint
{
    void Painter::SetNextColor(std::shared_ptr<Color> &color)
    {
        next_command_color_->SetColor(*color);
    }

    void Painter::DrawLine(const BasePoint &start,
                           const BasePoint &end,
                           const std::optional<std::shared_ptr<Color>> &line_color_,
                           const std::optional<Unit> &line_width_)
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point l_start = start.GetPointPX(dp->image_size_);
        Point l_end = end.GetPointPX(dp->image_size_);
        std::shared_ptr<Color> l_color = line_color_.value_or(next_command_color_);
        //Unit l_width = line_width_.value_or(1);
        float l_width_half = line_width_.value_or(1) / 2.0f;

        // TODO reverse the vector if it is in direction of [-1,-1]. It would break the algorithm if THE loop
        //      or do some checks/swaps in the loop
        // Calculate paameters of line equations
        line t{static_cast<float>(l_start.y - l_end.y),
               static_cast<float>(l_end.x - l_start.x),
               static_cast<float>(l_start.x * l_end.y - l_start.y * l_end.x)};

        // Create 2 parallel lines with distance of 1/2 l_width
        line t1(t);
        t1.c += l_width_half;

        line t2(t);
        t1.c -= l_width_half;

        vec2 size{dp->image_size_.x, dp->image_size_.y};
        vec2 y_bounds; // u is upper bound v is lower bound
        for (float x = 0.5; x < size.u; x += 1)
        {
            y_bounds.u = std::round(line_get_y(t1, x) - 0.5f);
            y_bounds.v = std::round(line_get_y(t2, x) - 0.5f);

            // Check bounds
            if (y_bounds.u > size.v)
                y_bounds.u = size.v;

            if (y_bounds.v < 0)
                y_bounds.u = 0;

            for (Unit y = y_bounds.v; y <= y_bounds.u; y++)
            {
                // line n{-t.a, t.b, 0.0f}; // Check distance (line_intersection & norm)
                std::copy_n(reinterpret_cast<uint8_t *>(l_color->GetData()), l_color->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * size.u + std::floor(x)]));
            }
        }

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::DrawCircle(const BasePoint &center,
                             const Unit &radius,
                             bool fill,
                             const std::optional<std::shared_ptr<Color>> &fill_color,
                             const std::optional<std::shared_ptr<Color>> &border_color,
                             const std::optional<Unit> &border_width)
    {
        throw;
        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::DrawBucket(const BasePoint &point, const std::optional<std::shared_ptr<Color>> &fill_color)
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point p = point.GetPointPX(dp->image_size_);
        Point image_size = dp->image_size_;
        int pixel_count = image_size.x * image_size.y;

        // Point outside of image, nothing to do
        if (p.x > image_size.x || p.y > image_size.y)
            return;

        std::vector<int> pixels_to_search(40);
        std::vector<int> pixels_to_change(40);
        pixels_to_change.push_back(p.y * dp->image_size_.x + p.x);

        std::unique_ptr<Color> picked_color(dp->GetColorType());
        picked_color->SetFromData((*dp)[pixels_to_change.front()]);
        size_t color_size_bytes(picked_color->GetDataSize());

        void *fill_data_ptr = fill_color.value_or(next_command_color_)->GetData();

        while (!pixels_to_search.empty() || !pixels_to_change.empty())
        {
            // Iterate through all found pixels with the same color in previous iteration
            std::for_each(pixels_to_change.begin(), pixels_to_change.end(), [&](auto &i) {
                // Change the color of the pixel to fill_color
                std::copy_n(reinterpret_cast<uint8_t *>(fill_data_ptr), color_size_bytes, reinterpret_cast<uint8_t *>((*dp)[i]));
            });

            // Clear pixels_to_search
            pixels_to_search.clear();

            // Iterate once again search around the pixel for pixels with desired color
            int idx;
            for (auto p : pixels_to_change)
            {
                // Check right pixel
                idx = p + 1;
                if (idx < pixel_count && std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.push_back(idx);

                // Check left pixel
                idx -= 2;
                if (idx > 0 && std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.push_back(idx);

                // Check top pixel
                idx = p - image_size.x;
                if (idx > 0 && std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.push_back(idx);

                // Check top pixel
                idx = p + image_size.x;
                if (idx > pixel_count && std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.push_back(idx);
            }

            // After changing all the pixels and searching new pixels to replace, swap pixels_to_search & pixels_to_change
            pixels_to_change.swap(pixels_to_search);
            pixels_to_search.clear();
        }

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::Crop(const BasePoint &corner1, const BasePoint &corner2)
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        Point c1 = corner1.GetPointPX(dp->image_size_);
        Point c2 = corner2.GetPointPX(dp->image_size_);

        // Set p1 as the top left point and p2 as the bottom right point
        Point p1{std::min(c1.x, c2.y), std::min(c1.y, c2.y)};
        Point p2{std::max(c1.x, c2.y), std::max(c1.y, c2.y)};

        // Both points are out of the bound -> error
        if (p1.x > image_size.x || p1.y > image_size.y || p2.x < 0 || p2.y < 0)
            throw;

        // If p1 is to the left or above the image -> move to the edge
        if (p1.x < 0)
            p1.x = 0;
        if (p1.y < 0)
            p1.y = 0;

        // If p2 is to the right or below the image -> move to the edge
        if (p2.x > image_size.x)
            p2.x = image_size.x;
        if (p2.y > image_size.y)
            p2.y = image_size.y;

        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{p2.x - p1.x, p2.y - p1.y}, dp->GetColorType());
        auto it = new_data_pixels->begin();
        auto it_end = new_data_pixels->end();
        Point new_size = new_data_pixels->image_size_;
        size_t color_size_bytes = dp->GetColorType()->GetDataSize();

        size_t x = p1.x;
        size_t y = p1.y;
        for (; it != it_end; it++, x++)
        {
            // Reached the end of line -> go to the 'beginning' (p1.x)
            if (x == new_size.x)
            {
                x = p1.x;
                y++;
            }

            std::copy_n(reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
        }

        dp.swap(new_data_pixels);

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::Resize(const BasePoint &new_size)
    {
        throw;
        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::Rotate(Rotation rotation)
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point image_size = dp->image_size_;
        size_t color_size_bytes = dp->GetColorType()->GetDataSize();
        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{image_size.y, image_size.x}, dp->GetColorType());
        auto it = new_data_pixels->begin();
        auto it_end = new_data_pixels->end();

        size_t pixel_count = image_size.x * image_size.y;

        // Clockwise rotation
        if (rotation == Rotation::kClock)
        {
            // Go up from bottom left, then to the right (inside old image)
            size_t idx = image_size.x * (image_size.y - 1);
            for (; it != it_end; it++, idx - image_size.x)
            {
                // Go back to the bottom, then one to the right
                if (idx < 0)
                    idx = idx + pixel_count + 1;

                // Reached the top right pixel of old image -> end
                if (idx == image_size.x - 1)
                {
                    std::copy_n(reinterpret_cast<uint8_t *>((*dp)[idx]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
                    break;
                }

                // Copy pixel
                std::copy_n(reinterpret_cast<uint8_t *>((*dp)[idx]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
            }
        }
        // Counter clockwise rotation
        else
        {
            // Go down from top right, then one to the left
            size_t idx = image_size.x - 1;
            for (; it != it_end; it++, idx + image_size.x)
            {
                // Go back to the top, then one to the left
                if (idx > pixel_count)
                    idx = idx - pixel_count - 1;

                // Reached bottom left pixel of old image -> end
                if (idx == pixel_count - image_size.x + 1)
                {
                    std::copy_n(reinterpret_cast<uint8_t *>((*dp)[idx]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
                    break;
                }

                // Copy pixel
                std::copy_n(reinterpret_cast<uint8_t *>((*dp)[idx]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
            }
        }

        dp.swap(new_data_pixels);

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::InvertColors()
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        std::unique_ptr<Color> color(dp->GetColorType());
        size_t color_size_bytes = color->GetDataSize();
        void *color_data = color->GetData();

        // TODO: Could implement subclass color reader/writer
        //      that would just iterate through the array and change the color,
        //      instead of copying there and back again just to invert color.
        std::for_each(dp->begin(), dp->end(), [&](auto p) {
            color->SetFromData(p);
            color->InvertColor();
            std::copy_n(reinterpret_cast<uint8_t *>(color_data), color_size_bytes, reinterpret_cast<uint8_t *>(p));
        });

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::ConvertToGrayscale()
    {
        throw;
        // Call back that image was edited
        image_edit_callback_();
    }
}