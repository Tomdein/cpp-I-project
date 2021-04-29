#ifndef PAINT_INC_IMAGE_IOBMP_H_
#define PAINT_INC_IMAGE_IOBMP_H_

#include <fstream>

#include "image_bmp.h"

namespace paint
{
    namespace image_bmp
    {
        /**
         * @brief This class contains methods to read and write BMP images.
         * 
         * This class implemments static methods for reading and writing different parts of the image file.
         * 
         */
        class ImageIOBMP
        {
        public:
            static void ReadHeaderBMP(std::ifstream &file, ImageBMP &image);
            static void ReadHeaderBMPInfo(std::ifstream &file, ImageBMP &image);
            static void ReadPixelData(std::ifstream &file, ImageBMP &image);

            static void WriteHeaderBMP(std::ofstream &file, ImageBMP &image);
            static void WriteHeaderBMPInfo(std::ofstream &file, ImageBMP &image);
            static void WriteColorTable(std::ofstream &file, ImageBMP &image);
            static void WritePixelData(std::ofstream &file, ImageBMP &image);

        private:
            ImageIOBMP() {}
        };
    }
}

#endif // PAINT_INC_IMAGE_IOBMP_H_