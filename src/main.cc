#include <iostream>
#include <memory>
#include <optional>

#include "image_bmp.h"
#include "file.h"
#include "colors.h"

int main()
{
    paint::image_bmp::ImageBMP image("../../images/w3c_home.png");
    image.LoadImage();

    //image.SaveImage(); //weirdo error
    image.painter.Crop(paint::PointPX(3, 6), paint::PointPX(63, 42));
    image.painter.Rotate(paint::Rotation::kClock);
    image.painter.Resize(paint::PointPX(18, 30));
    //image.painter.Resize(paint::PointPX(36, 24)); // Shifted by a pixel up
    image.painter.DrawBucket(paint::PointPX(0, 0), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(255, 0, 0))); //Changes only one pixel to BLUE?
    image.painter.InvertColors();

    image.SaveBuffer();

    image.SetOutputImage(paint::File{paint::FileType::kBMP, "../../images/out.png"});
    image.SaveImage();

    // std::cout << sizeof(paint::image_bmp::HeaderBMP) << ", " << sizeof(paint::image_bmp::HeaderBMPInfo) << '\n';
}