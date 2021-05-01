#ifndef PAINT_INC_IMAGE_IOPNG_H_
#define PAINT_INC_IMAGE_IOPNG_H_

#include <fstream>

#include "image_png.h"

namespace paint
{
    namespace image_png
    {
        /**
         * @brief This class contains methods to read and write PNG images.
         * 
         * This class implemments static methods for reading and writing different parts of the image file.
         * 
         */
        class ImageIOPNG
        {
        public:
            static void ReadChunkPNG(std::ifstream &file, ImagePNG &image);

            static void WriteChunkPNG(std::ofstream &file, ImagePNG &image);

        private:
            ImageIOPNG() {}
        };
    }
}

#endif // PAINT_INC_IMAGE_IOPNG_H_