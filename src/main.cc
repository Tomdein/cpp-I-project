#include <iostream>
#include <memory>
#include <optional>

#include "image_bmp.h"
#include "file.h"
#include "colors.h"
#include "point.h"

int main()
{
    paint::image_bmp::ImageBMP image("../../images/w3c_home.bmp");
    image.LoadImage();
    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out.bmp"});

    image.painter.Crop(paint::PointPX(3, 6), paint::PointPX(63, 42));

    image.painter.Rotate(paint::Rotation::kClock);

    image.painter.Resize(paint::PointPX(18, 30));
    // image.painter.Resize(paint::PointPX(16, 28));
    // image.painter.Resize(paint::PointPX(36, 24));

    // image.painter.DrawLine(paint::PointPX(17, 0), paint::PointPX(0, 29), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 255, 0)), std::make_optional<paint::Unit>(3)); \\ throws
    image.painter.DrawLine(paint::PointPX(0, 0), paint::PointPX(18, 30), std::make_optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(0, 255, 0)), std::make_optional<paint::Unit>(3));

    image.painter.DrawBucket(paint::PointPX(0, 1), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(255, 0, 0)));

    image.painter.InvertColors();

    image.SaveImage();
}