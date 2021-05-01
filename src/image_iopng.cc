#include "image_iopng.h"

#include <vector>
#include <algorithm>

namespace paint
{
    namespace image_png
    {
        void ImageIOPNG::ReadChunkPNG(std::ifstream &file, ImagePNG &image)
        {
            uint64_t length;
            file.read(reinterpret_cast<char *>(&length), sizeof(length));

            uint64_t type;
            file.read(reinterpret_cast<char *>(&type), sizeof(type));

            std::shared_ptr<uint64_t[]> data(new uint64_t[length]);
            file.read(reinterpret_cast<char *>(data.get()), length);

            std::vector<uint64_t> vec_data();

            // std::transform(data, data.get() + length, std::back_inserter(vec_data), [auto x]() { return __builtin_bswap64 })

            uint64_t crc;
            file.read(reinterpret_cast<char *>(&crc), sizeof(crc));
        }

        void ImageIOPNG::WriteChunkPNG(std::ofstream &file, ImagePNG &image)
        {
        }
    }
}