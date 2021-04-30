#include <iostream>
#include <memory>
#include <optional>

#include "image_bmp.h"
#include "painter.h"
#include "file.h"
#include "colors.h"
#include "point.h"

int main()
{
    paint::image_bmp::ImageBMP image("../../images/w3c_home.bmp");

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_crop.bmp"});
    image.painter.Crop(paint::PointPX(3, 6), paint::PointPX(63, 42));
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_rotate_clock.bmp"});
    image.painter.Rotate(paint::Rotation::kClock);
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_resize.bmp"});
    image.painter.Resize(paint::PointPX(18, 30));
    // image.painter.Resize(paint::PointPX(16, 28));
    // image.painter.Resize(paint::PointPX(36, 24));
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_line.bmp"});
    image.painter.DrawLine(paint::PointPX(17, 0), paint::PointPX(0, 29), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 255, 0)), std::make_optional<paint::Unit>(5));
    image.painter.DrawLine(paint::PointPX(5, 7), paint::PointPX(15, 17), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(127, 127, 127)), std::make_optional<paint::Unit>(2));
    // image.painter.DrawLine(paint::PointPX(0, 0), paint::PointPX(18, 30), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 255, 0)), std::make_optional<paint::Unit>(3));
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_bucket.bmp"});
    image.painter.DrawBucket(paint::PointPX(0, 1), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(255, 0, 0)));
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_invert_colors.bmp"});
    image.painter.InvertColors();
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_grayscale.bmp"});
    image.painter.ConvertToGrayscale();
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_b&w.bmp"});
    image.painter.ConvertToBW();
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out_b&w_removed_bg.bmp"});
    image.painter.DrawBucket(paint::PointPX(0, 1), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 0, 0)));
    image.painter.ConvertToBW();
    image.SaveImage();

    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out.bmp"});
    image.painter.Crop(paint::PointPX(3, 6), paint::PointPX(63, 42));
    image.painter.Rotate(paint::Rotation::kClock);
    image.painter.Resize(paint::PointPX(18, 30));
    image.painter.DrawLine(paint::PointPX(17, 0), paint::PointPX(0, 29), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 255, 0)), std::make_optional<paint::Unit>(5));
    image.painter.DrawLine(paint::PointPX(5, 7), paint::PointPX(15, 17), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(127, 127, 127)), std::make_optional<paint::Unit>(2));
    image.painter.DrawBucket(paint::PointPX(0, 1), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(255, 0, 0)));
    image.painter.InvertColors();
    image.painter.ConvertToGrayscale();
    image.SaveImage();
}