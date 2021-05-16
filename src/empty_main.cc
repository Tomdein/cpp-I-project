#include <iostream>
#include <fstream>

#include "parser.h"
#include "image_bmp.h"

int main(int, char *[])
{
    paint::image_bmp::ImageBMP image_bw(std::filesystem::path("images/out_b&w.bmp"));
    image_bw.LoadImage();
    image_bw.SaveImage("images/resaved_bw.bmp");

    // std::cout << sizeof(paint::grayscale_palette) << std::endl;

    // std::ofstream f("../../src/dummy.cc", std::ios_base::out);

    // f << "{";

    // for (int i = 0; i < 256; i++)
    // {
    //     f << "{" << i << ", " << i << ", " << i << "},\n";
    // }

    // f << "}";
}