#include "painter.h"
#include "color_grayscale.h"
#include "color_bw.h"
#include "unit.h"
#include "vec.h"

#include <set>
#include <cstring>
#include <iostream>

namespace paint
{
    void Painter::SetNextColor(const std::shared_ptr<Color> &color)
    {
        next_command_color_->SetColor(*color);
    }

    void Painter::ClearImage(const std::optional<std::shared_ptr<Color>> &clear_color)
    {
        std::shared_ptr<Color> color = clear_color.value_or(std::make_shared<ColorRGB888>(255, 255, 255));

        if (image_data_.expired())
        {
            std::cerr << "Painter::DrawLine - image_data_.expired!" << std::endl;
            throw "image_data_.expired";
        }

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        // Init color
        auto fill_color = dp->GetColorType();
        fill_color->SetColor(*clear_color.value_or(next_command_color_));
        auto fill_color_data = reinterpret_cast<uint8_t *>(fill_color->GetData());
        auto fill_color_data_size = fill_color->GetDataSize();

        for (auto data_it : *dp)
        {
            std::copy_n(fill_color_data, fill_color_data_size, reinterpret_cast<uint8_t *>(data_it));
        }
    }

    void Painter::DrawLine(const BasePoint &start,
                           const BasePoint &end,
                           const std::optional<std::shared_ptr<Color>> &line_color_,
                           const std::optional<Unit> &line_width_)
    {
        if (image_data_.expired())
        {
            std::cerr << "Painter::DrawLine - image_data_.expired!" << std::endl;
            throw "image_data_.expired";
        }

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        // Get line points and width
        Point l_start = start.GetPointPX(dp->image_size_);
        Point l_end = end.GetPointPX(dp->image_size_);
        float l_width_half = line_width_.value_or(1) / 2.0f;

        // If image data is horizontaly mirrored -> flip line horizontaly
        if (draw_bottom_up_)
        {
            l_start = Point{l_start.x, dp->image_size_.y - l_start.y};
            l_end = Point{l_end.x, dp->image_size_.y - l_end.y};
        }

        // Init Color
        auto l_color = dp->GetColorType();
        l_color->SetColor(*line_color_.value_or(next_command_color_));

        //TODO: reverse the vector if it is in direction of [-1,-1]. It would break the algorithm if THE loop
        //      or do some checks/swaps in the loop
        // Calculate parameters of line equations
        line t{static_cast<float>(l_start.y - l_end.y),
               static_cast<float>(l_end.x - l_start.x),
               static_cast<float>(l_start.x * l_end.y - l_start.y * l_end.x)};

        // Create 2 parallel lines with distance of 1/2 l_width
        // Point (x, y + l_width_half)
        line t1(t);
        t1.c -= t1.b * l_width_half;

        // Point (x, y - l_width_half)
        line t2(t);
        t2.c += t2.b * l_width_half;

        vec2 size{dp->image_size_.x, dp->image_size_.y};
        vec2 y_bounds; // u is upper bound v is lower bound

        // Set start and end x coordinates
        int x = l_start.x;
        int x_end = l_end.x;

        // Set x to be on the left
        if (x > x_end)
        {
            std::swap(x, x_end);
        }

        // Check bounds of left (starting) x
        if (x < 0)
            x = 0;

        // Check bounds of right (ending) x
        if (x_end > size.u)
            x_end = size.u;

        // For each x fill calculated y points
        for (; x < x_end; x += 1)
        {
            y_bounds.u = std::round(line_get_y(t1, x));
            y_bounds.v = std::round(line_get_y(t2, x));

            // Check bounds of max (top) y
            if (y_bounds.u > size.v)
                y_bounds.u = size.v;

            // Check bounds of min (bottom) y
            if (y_bounds.v < 0)
                y_bounds.v = 0;

            for (Unit y = y_bounds.v; y <= y_bounds.u && y < size.v; y++)
            {
                // line n{-t.a, t.b, 0.0f}; // Check distance (line_intersection & norm)
                std::copy_n(reinterpret_cast<uint8_t *>(l_color->GetData()), l_color->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * size.u + x]));
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
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        // Get parameters
        Point cen = center.GetPointPX(image_size);
        Unit border_w = border_width.value_or(1);

        // If image data is horizontaly mirrored -> flip licircle center horizontaly
        if (draw_bottom_up_)
        {
            cen = Point{cen.x, dp->image_size_.y - cen.y};
        }

        // Init colors
        auto fill_c = dp->GetColorType();
        fill_c->SetColor(*fill_color.value_or(next_command_color_));

        auto border_c = dp->GetColorType();
        border_c->SetColor(*border_color.value_or(next_command_color_));

        // Create circles
        Circle c_outer{cen, radius + static_cast<int>(std::round(static_cast<float>(border_w) / 2.0f))};
        Circle c_inner{cen, radius - static_cast<int>(std::round(static_cast<float>(border_w) / 2.0f))};

        Unit x1 = c_outer.center.x - c_outer.radius;
        Unit x2 = c_inner.center.x - c_inner.radius;

        Unit y1;
        Unit y2;

        // Set y1 to y coordinate of circle
        y1 = c_outer.center.y;

        // Color circular segments to the left and right
        for (Unit x = x1; x < x2; x++)
        {
            // Get max y
            y2 = circle_get_top_y(c_outer, x);

            // TODO: y < y2 or y <= y2
            for (Unit y = y1; y < y2; y++)
            {
                // Use symetry to color the 4 pixels
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
            }
        }

        // Color the rest of the circle
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
                    // Use symetry to color the 4 pixels
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                    std::copy_n(reinterpret_cast<uint8_t *>(fill_c->GetData()), fill_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
                }
            }

            // Fill the rest of pixels between c_inner & c_outer
            // TODO: y < y2 or y <= y2
            for (Unit y = y1; y < y2; y++)
            {
                // Use symetry to color the 4 pixels
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[y * image_size.x + (c_outer.center.x << 2) - x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + x]));
                std::copy_n(reinterpret_cast<uint8_t *>(border_c->GetData()), border_c->GetDataSize(), reinterpret_cast<uint8_t *>((*dp)[((c_outer.center.y << 2) - y) * image_size.x + (c_outer.center.x << 2) - x]));
            }
        }
        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::DrawBucket(const BasePoint &point, const std::optional<std::shared_ptr<Color>> &fill_color_in)
    {
        if (image_data_.expired())
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point p = point.GetPointPX(dp->image_size_);
        Point image_size = dp->image_size_;
        int pixel_count = image_size.x * image_size.y;

        // Point outside of image, nothing to do
        if (p.x < 0 || p.x >= image_size.x || p.y < 0 || p.y >= image_size.y)
            return;

        // If image data is horizontaly mirrored -> flip starting point horizontaly
        if (draw_bottom_up_)
        {
            p = Point{p.x, dp->image_size_.y - p.y};
        }

        // Init 2 sets used for breadth search
        std::set<int> pixels_to_search;
        std::set<int> pixels_to_change;
        int first_idx = p.y * dp->image_size_.x + p.x;
        pixels_to_change.insert(first_idx);

        // Init the color
        std::unique_ptr<Color> picked_color(dp->GetColorType());
        picked_color->SetFromData((*dp)[first_idx]);
        size_t color_size_bytes(picked_color->GetDataSize());

        auto fill_color = dp->GetColorType();
        fill_color->SetColor(*fill_color_in.value_or(next_command_color_));
        auto fill_data_ptr = reinterpret_cast<uint8_t *>(fill_color->GetData());

        // Search pixels to replace
        while (!pixels_to_search.empty() || !pixels_to_change.empty())
        {
            // Iterate through all found pixels with the same color in previous iteration
            std::for_each(pixels_to_change.begin(), pixels_to_change.end(), [&](auto &i) {
                // Change the color of the pixel to fill_color
                std::copy_n(fill_data_ptr, color_size_bytes, reinterpret_cast<uint8_t *>((*dp)[i]));
            });

            // Clear pixels_to_search
            pixels_to_search.clear();

            // Iterate once again search around the pixel for pixels with desired color
            int idx;
            for (auto p : pixels_to_change)
            {
                // Check right pixel
                idx = p + 1;
                if (idx < pixel_count && p % image_size.x != image_size.x - 1 && !std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.insert(idx);

                // Check left pixel
                idx -= 2;
                if (idx >= 0 && p % image_size.x != 0 && !std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.insert(idx);

                // Check top pixel
                idx = p - image_size.x;
                if (idx >= 0 && !std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.insert(idx);

                // Check top pixel
                idx = p + image_size.x;
                if (idx < pixel_count && !std::memcmp((*dp)[idx], picked_color->GetData(), color_size_bytes))
                    pixels_to_search.insert(idx);
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
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        // Retrieve unit in PX
        Point c1 = corner1.GetPointPX(dp->image_size_);
        Point c2 = corner2.GetPointPX(dp->image_size_);

        // If image data is horizontaly mirrored -> flip starting points horizontaly
        if (draw_bottom_up_)
        {
            c1 = Point{c1.x, dp->image_size_.y - c1.y};
            c2 = Point{c2.x, dp->image_size_.y - c2.y};
        }

        // Set p1 as the top left point and p2 as the bottom right point
        Point p1{std::min(c1.x, c2.x), std::min(c1.y, c2.y)};
        Point p2{std::max(c1.x, c2.x), std::max(c1.y, c2.y)};

        // Both points are out of the bound -> error (|| because p1 should be the left top point and p2 should be bottom right point)
        if ((p1.x > image_size.x && p1.y > image_size.y) || (p2.x < 0 && p2.y < 0))
            throw "image_data_.expired";

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

        // Set new data
        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{p2.x - p1.x, p2.y - p1.y}, dp->GetColorType());
        Point new_size = new_data_pixels->image_size_;
        size_t color_size_bytes = dp->GetColorType()->GetDataSize();

        // Set iterators
        auto it = new_data_pixels->begin();
        auto it_end = new_data_pixels->end();

        // For each pixel in new data -> set data from old data
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

            size_t idx = y * image_size.x + x;
            std::copy_n(reinterpret_cast<uint8_t *>((*dp)[idx]), color_size_bytes, reinterpret_cast<uint8_t *>(*it));
        }

        // Swap the new and old data
        dp->SwapData(*new_data_pixels);

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::Resize(const BasePoint &new_size)
    {
        if (image_data_.expired())
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();
        Point image_size = dp->image_size_;

        Point new_image_size = new_size.GetPointPX(image_size);

        // Desired image has the same size -> no resize needed
        if (new_image_size == image_size)
            return;

        vec2f multiplier{static_cast<float>(image_size.x) / static_cast<float>(new_image_size.x),
                         static_cast<float>(image_size.y) / static_cast<float>(new_image_size.y)};

        vec2f point_in_old;

        // Create new data
        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{new_image_size.x, new_image_size.y}, dp->GetColorType());

        // Letf, right, top and bottom points used for bilinear interpolation
        Unit p_left;
        Unit p_right;
        Unit p_top;
        Unit p_bottom;

        // Init colors for 4 points
        std::shared_ptr<Color> c1(dp->GetColorType().release());
        std::shared_ptr<Color> c2(dp->GetColorType().release());
        std::shared_ptr<Color> c_interp_bottom(dp->GetColorType().release());
        std::shared_ptr<Color> c_interp_top(dp->GetColorType().release());

        // For each pixel in new data
        for (int y = 0; y != new_image_size.y; y++)
        {
            for (int x = 0; x != new_image_size.x; x++)
            {
                // Get pixel position in old image
                point_in_old = vec2f{x * multiplier.u, y * multiplier.v};

                // Find surrounding pixels
                p_left = std::floor(point_in_old.u);
                p_right = std::ceil(point_in_old.u);
                p_top = std::floor(point_in_old.v); // Top point is at (x, 0)!!!
                p_bottom = std::ceil(point_in_old.v);

                if (p_left != p_right)
                {
                    // Start with bottom points. Set them to their color
                    c1->SetFromData((*dp)[p_bottom * image_size.x + p_left]);
                    c2->SetFromData((*dp)[p_bottom * image_size.x + p_right]);

                    // Interpolate bottom points
                    c_interp_bottom->Interpolate(c1, c2, 1 - (point_in_old.u - p_left));
                }
                else
                {
                    c1->SetFromData((*dp)[p_bottom * image_size.x + p_left]);
                    c_interp_bottom->SetColor(*c1);
                }

                if (p_top != p_bottom)
                {
                    // Then set top points to their color
                    c1->SetFromData((*dp)[p_top * image_size.x + p_left]);
                    c2->SetFromData((*dp)[p_top * image_size.x + p_right]);

                    // Interpolate top points
                    c_interp_top->Interpolate(c1, c2, 1 - (point_in_old.u - p_left));
                }
                else
                {
                    c1->SetFromData((*dp)[p_top * image_size.x + p_left]);
                    c_interp_top->SetColor(*c1);
                }

                // Now interpolate the c_interp_bottom & c_interp_top in y direction
                c1->Interpolate(c_interp_top, c_interp_bottom, 1 - (point_in_old.v - p_top));
                //c1->SetColor(ColorRGB888(0, 0, 0));

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
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        Point image_size = dp->image_size_;
        size_t color_size_bytes = dp->GetColorType()->GetDataSize();

        // Create new data
        std::shared_ptr<DataPixels> new_data_pixels = std::make_shared<DataPixels>(Point{image_size.y, image_size.x}, dp->GetColorType());

        // Create iterators to new data
        auto it = new_data_pixels->begin();
        auto it_end = new_data_pixels->end();

        int pixel_count = image_size.x * image_size.y;

        // Clockwise rotation for 'bottom up' images, Counter Clockwise rotation for 'top down' images
        if ((rotation == Rotation::kClock) ^ draw_bottom_up_)
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
        // Counter Clockwise rotation for 'bottom up' images, Clockwise rotation for 'top down' images
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
                if (idx == pixel_count - image_size.x)
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
            throw "image_data_.expired";

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
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        dp->TransformToColorType(std::unique_ptr<Color>(new ColorGrayscale(0)));

        // Call back that image was edited
        image_edit_callback_();
    }

    void Painter::ConvertToBW()
    {
        if (image_data_.expired())
            throw "image_data_.expired";

        // Lock the image data
        std::shared_ptr<DataPixels> dp = image_data_.lock();

        dp->TransformToColorType(std::unique_ptr<Color>(new ColorBW(0)));

        // Call back that image was edited
        image_edit_callback_();
    }
}