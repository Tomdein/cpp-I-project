#ifndef PAINT_INC_IMAGE_PNG_H_
#define PAINT_INC_IMAGE_PNG_H_

#include <cstdint>
#include <vector>

#include "image.h"
#include "image_png_chunk.h"

namespace paint
{
    namespace image_png
    {

        /**
         * @brief Signature of the file signifying it is a PNG picture.
         * 
         */
        const uint64_t kPNGSignature = 0x0a1a0a0d474e5089ULL;

        class ImagePNG : public Image
        {
        public:
            ImagePNG() = default;
            virtual ~ImagePNG() override {}

            virtual void CreateImage() override {}
            virtual void LoadImage() override {}
            virtual void SaveImage() override {}
            virtual void GenerateMetadata() override {}

        private:
            std::shared_ptr<ChunkIHDR> chunk_ihdr_;
            std::shared_ptr<ChunkPLTE> chunk_plte_;
            std::vector<std::shared_ptr<ChunkIDAT>> vec_chunk_data_;
            std::shared_ptr<ChunkIEND> chunk_iend_;
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_H_