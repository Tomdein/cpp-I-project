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
            static void ReadPNGFile(std::ifstream &file, ImagePNG &img);
            static std::shared_ptr<ChunkPNG> ReadChunkPNG(std::ifstream &file);
            static void ReadIDATChunks(ImagePNG &img);

            static void WriteChunkPNG(std::ofstream &file, std::shared_ptr<ChunkPNG> chunk);
            static void GenerateIHDRChunks(ImagePNG &img);
            static void GenerateIDATChunks(ImagePNG &img);

        private:
            ImageIOPNG(){};

            static unsigned int ValuesInScanline(const std::unique_ptr<int[]> &scanline, int bytes);
        };
    }
}

#endif // PAINT_INC_IMAGE_IOPNG_H_