#ifndef PAINT_INC_IMAGE_IOPNG_H_
#define PAINT_INC_IMAGE_IOPNG_H_

#include <fstream>

#include "image_png.h"

namespace paint
{
    namespace image_png
    {
        class ChunkPNG;

        /**
         * @brief This class contains methods to read and write PNG images.
         * 
         * This class implemments static methods for reading and writing different parts of the image file.
         * 
         */
        class ImageIOPNG
        {
        public:
            static std::shared_ptr<ChunkPNG> ReadChunkPNG(std::ifstream &file);

            static void WriteChunkPNG(std::ofstream &file, std::shared_ptr<ChunkPNG> &chunk);

        private:
            ImageIOPNG() {}
        };
    }
}

#endif // PAINT_INC_IMAGE_IOPNG_H_