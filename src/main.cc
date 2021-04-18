#include <iostream>
#include <memory>

#include "image_bmp.h"
#include "colors.h"

int main()
{
    std::cout << sizeof(paint::ColorRGB888) << '\n';

    //paint::Color *p = new paint::ColorRGB888(0, 0, 0);
    //paint::ColorRGB888 *p_orig = new paint::ColorRGB888(0, 0, 0);
    delete new paint::image_bmp::ImageBMP("../../images/dummy");
    delete new paint::ColorRGB888(0, 0, 0);
    //delete p_orig;

    //std::shared_ptr<paint::Image> i1 = std::shared_ptr<paint::Image>(new paint::image_bmp::ImageBMP("../../images/dummy"));

    //std::shared_ptr<paint::Color> c1 = std::make_shared<paint::ColorRGB888>(0, 0, 0);
    //std::shared_ptr<paint::Color> c2 = std::shared_ptr<paint::Color>(c1->clone2());
    //std::shared_ptr<paint::Color> c2 = std::shared_ptr<paint::Color>(new paint::ColorRGB565(0, 0, 0));

    // paint::image_bmp::ImageBMP image("../../images/w3c_home.png");
    // image.LoadImage();
    // image.painter.Rotate(paint::Rotation::kClock);
    // image.SaveImage();

    // std::cout << sizeof(paint::image_bmp::HeaderBMP) << ", " << sizeof(paint::image_bmp::HeaderBMPInfo) << '\n';
}