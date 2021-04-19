#include <iostream>
#include <memory>
#include <optional>

#include "image_bmp.h"
#include "colors.h"

int main()
{
    paint::image_bmp::ImageBMP image("../../images/w3c_home.png");
    image.LoadImage();
    //image.SaveImage(); //wierdo error
    image.painter.Crop(paint::PointPX(3, 6), paint::PointPX(63, 42));
    image.painter.Rotate(paint::Rotation::kClock);
    image.painter.Resize(paint::PointPX(18, 30));
    //image.painter.Resize(paint::PointPX(36, 24)); // Shifted by a pixel up
    image.painter.DrawBucket(paint::PointPX(0, 0), std::optional<std::shared_ptr<paint::Color>>(new paint::ColorRGB888(255, 0, 0))); //Changes only one pixel to BLUE?
    image.painter.InvertColors();
    image.SaveBuffer();
    //image.SaveImage();

    // std::cout << sizeof(paint::image_bmp::HeaderBMP) << ", " << sizeof(paint::image_bmp::HeaderBMPInfo) << '\n';
}