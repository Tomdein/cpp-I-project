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
    class Image; /// Forward declaration of class Image.

    /**
     * @brief Object used for editing image data.
     * 
     * Image object contains one public Painter object that is used to edit the image data, i.e.: drawing, rotation, resizing, ...
     * 
     * This class is a friend of DataPixels, so that it can edit the image pixels.
     * 
     */
    class Painter
    {
    public:
        Painter(std::function<void()> image_edit_callback) : image_edit_callback_{std::move(image_edit_callback)} {}

        /**
         * @brief Connects Painter with image data.
         * 
         * @param image_data a DataPixels used for editing.
         */
        void AttachImageData(const std::weak_ptr<DataPixels> &image_data)
        {
            image_data_ = image_data;
        }
        /**
         * @brief Disconnects Painter from image data.
         * 
         */
        void DetachImageData()
        {
            image_data_.reset();
        }

        /**
         * @brief Sets the global color.
         * 
         * Sets the global color that is used for drawing when no color is specified when drawing.
         * 
         * @param color new global color.
         */
        void SetNextColor(std::shared_ptr<Color> &color);

        /**
         * @brief Draws a line from start to end.
         * 
         * @param start start point.
         * @param end end point.
         * @param line_color_ color of the line (default = global color).
         * @param line_width_ width of the line (default = 1px).
         */
        virtual void DrawLine(const BasePoint &start,
                              const BasePoint &end,
                              const std::optional<std::shared_ptr<Color>> &line_color_ = std::nullopt,
                              const std::optional<Unit> &line_width_ = std::nullopt);
        /**
         * @brief Draws a circle.
         * 
         * @param center center of the circle.
         * @param radius radius of the circle.
         * @param fill to fill or not to fill the circle (default = false).
         * @param fill_color color of the fill (default = global color).
         * @param border_color color of the circle edge (default = global color).
         * @param border_width width of the circle edge (default = 1px).
         */
        virtual void DrawCircle(const BasePoint &center,
                                const Unit &radius,
                                bool fill = false,
                                const std::optional<std::shared_ptr<Color>> &fill_color = std::nullopt,
                                const std::optional<std::shared_ptr<Color>> &border_color = std::nullopt,
                                const std::optional<Unit> &border_width = std::nullopt);
        /**
         * @brief Fills all the connected pixels.
         * 
         * Finds all connected pixels with the color of the pixel located at point.
         * A connected pixel must touch an edge of another pixel -> not through a corner.
         * 
         * @param point point where to start and pick a color.
         * @param fill_color color with which to replace selected color (default = global color).
         */
        virtual void DrawBucket(const BasePoint &point, const std::optional<std::shared_ptr<Color>> &fill_color = std::nullopt);

        // Crop, Resize and Rotate modifies the headers
        /**
         * @brief Crops the image.
         * 
         * TODO: If crop size is bigger than original size, do not crop the image.
         * 
         * @param corner1 first point used to define the crop area.
         * @param corner2 second point used to define the crop area.
         */
        virtual void Crop(const BasePoint &corner1, const BasePoint &corner2);
        /**
         * @brief Resizes the image.
         * 
         * Resizes the image using bilinear interpolation.
         * 
         * @param new_size target size.
         */
        virtual void Resize(const BasePoint &new_size);
        /**
         * @brief Rotates the image.
         * 
         * Rotates the image by 90 degrees clock or counter clockwise.
         * 
         * @param rotation direction of the rotation.
         */
        virtual void Rotate(Rotation rotation);

        // InvertColors and ConvertToGrayscale *CAN* modify the headers
        /**
         * @brief Inverts the color.
         *  
         * Changes every pixel color to its inverse.
         * 
         */

        virtual void InvertColors();
        /**
         * @brief Converts the color to grayscale.
         * 
         * Converts every pixel to grayscale color.
         * 
         */
        virtual void ConvertToGrayscale();
        /**
         * @brief Converts the color to black & white.
         * 
         * Converts every pixel to black & white color.
         * 
         */
        virtual void ConvertToBW();

    private:
        std::shared_ptr<Color> next_command_color_; /// Global color used when no color is specified.
        std::weak_ptr<DataPixels> image_data_;      /// Data of the image to draw in.

        std::function<void()> image_edit_callback_; /// Imgage::ImageEditCallback() used to notify Image of the data change after editing.
    };
}

#endif // PAINT_INC_PAINTER_H_