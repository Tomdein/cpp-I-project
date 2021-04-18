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
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        Point cen = center.GetPointPX(image_size);
        std::shared_ptr<Color> fill_c(fill_color.value_or(next_command_color_));
        std::shared_ptr<Color> border_c(border_color.value_or(next_command_color_));
        Unit border_w = border_width.value_or(1);

        Circle c_outer{cen, radius + static_cast<int>(std::round(static_cast<float>(border_w) / 2.0f))};
        Circle c_inner{cen, radius - static_cast<int>(std::round(static_cast<float>(border_w) / 2.0f))};

        Unit x1 = c_outer.center.x - c_outer.radius;
        Unit x2 = c_inner.center.x - c_inner.radius;

        Unit y1;
        Unit y2;

        // Set y1 to y coordinate of circle
        y1 = c_outer.center.y;
        for (Unit x = x1; x < x2; x++)
        {
            // Get max y
            y2 = circle_get_top_y(c_outer, x);

            // TODO: y < y2 or y <= y2
            for (Unit y = y1; y < y2; y++)
            {
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
            }
        }

        for (Unit x = x1; x < x2; x++)
        {
            // Get max y
            y2 = circle_get_top_y(c_outer, x);
            y1 = circle_get_top_y(c_inner, x);

            // Fill all pixels from center to c_inner
            if (fill)
            {
                // TODO: y < y2 or y <= y2
                for (Unit y = 0; y < y1; y++)
                {
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
                }
            }

            // Fill all pixels between c_inner & c_outer
            // TODO: y < y2 or y <= y2
            for (Unit y = y1; y < y2; y++)
            {
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
            }
        }
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

        int x = p1.x;
        int y = p1.y;
        for (; it != it_end; it++, x++)
        {
            // Reached the end of line -> go to the 'beginning' (p1.x)
            if (x == p1.x + new_size.x)
            {
                x = p1.x;
                y++;
            }

            std::copy_n(reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
        }

        // Swap the new and old data
        dp->SwapData(*new_data_pixels);

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::Resize(const BasePoint &new_size)
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        Point new_image_size = new_size.GetPointPX(image_size);
        vec2f multiplier{static_cast<float>(image_size.x) / static_cast<float>(new_image_size.x),
                         static_cast<float>(image_size.y) / static_cast<float>(new_image_size.y)};

        vec2f point_in_old;

        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{new_image_size.x, new_image_size.y}, dp->GetColorType());

        // Letf, right, top and bottom points used for bilinear interpolation
        Unit p_left;
        Unit p_right;
        Unit p_top;
        Unit p_bottom;

        std::shared_ptr<Color> c1(dp->GetColorType().release());
        std::shared_ptr<Color> c2(dp->GetColorType().release());
        std::shared_ptr<Color> c_interp_bottom(dp->GetColorType().release());
        std::shared_ptr<Color> c_interp_top(dp->GetColorType().release());

        for (int y = 0; y != image_size.y; y++)
        {
            for (int x = 0; x != image_size.y; x++)
            {
                point_in_old = vec2f{x * multiplier.u, y * multiplier.v};

                p_left = std::floor(point_in_old.u);
                p_right = std::ceil(point_in_old.u);
                p_top = std::floor(point_in_old.v); // Top point is at (x, 0)!!!
                p_bottom = std::ceil(point_in_old.v);

                // Start with bottom points. Set them to their color
                c1->SetFromData((*dp)[p_bottom * image_size.x + p_left]);
                c2->SetFromData((*dp)[p_bottom * image_size.x + p_right]);

                // Interpolate bottom points
                c_interp_bottom->Interpolate(c1, c2, point_in_old.u - p_left);

                // Then set top points to their color
                c1->SetFromData((*dp)[p_top * image_size.x + p_left]);
                c2->SetFromData((*dp)[p_top * image_size.x + p_right]);

                // Interpolate top points
                c_interp_top->Interpolate(c1, c2, point_in_old.u - p_left);

                // Now interpolate the c_interp_bottom & c_interp_top in y direction
                c1->Interpolate(c_interp_top, c_interp_bottom, point_in_old.v - p_top);

                // Copy the color data
                std::copy_n(reinterpret_cast<uint8_t *>(c1->GetData()), c1->GetDataSize(), reinterpret_cast<uint8_t *>((*new_data_pixels)[y * new_image_size.x + x]));
            }
        }

        // Swap the new and old data
        dp->SwapData(*new_data_pixels);

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

        int pixel_count = image_size.x * image_size.y;

        // Clockwise rotation
        if (rotation == Rotation::kClock)
        {
            // Go up from bottom left, then to the right (inside old image)
            int idx = image_size.x * (image_size.y - 1);
            for (; it != it_end; it++, idx -= image_size.x)
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
            int idx = image_size.x - 1;
            for (; it != it_end; it++, idx += image_size.x)
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

        // Swap the new and old data
        dp->SwapData(*new_data_pixels);

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::InvertColors()
    {
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

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
        if (image_data_.expired())
            throw;

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point image_size = dp->image_size_;
        std::shared_ptr<Color> old_color(dp->GetColorType());
        //size_t old_color_size_bytes = old_color->GetDataSize();

        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{image_size.y, image_size.x}, dp->GetColorType());
        std::shared_ptr<Color> new_color;
        size_t new_color_size_bytes = new_color->GetDataSize();

        int pixel_count = image_size.x * image_size.y;

        for (int idx = 0; idx < pixel_count; idx++)
        {
            // Copy data from pixel to color
            old_color->SetFromData((*dp)[idx]);
            new_color->SetColor(*old_color);
            std::copy_n(reinterpret_cast<uint8_t *>(new_color->GetData()), new_color_size_bytes, reinterpret_cast<uint8_t *>((*dp)[idx]));
        }

        throw;
        // Call back that image was edited
        image_edit_callback_();
    }
}