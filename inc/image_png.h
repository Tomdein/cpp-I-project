#ifndef PAINT_INC_IMAGE_PNG_H_
#define PAINT_INC_IMAGE_PNG_H_

#include <cstdint>

#include "image.h"

namespace paint
{
    namespace image_png
    {

        /**
         * @brief Signature of the file signifying it is a PNG picture.
         * 
         */
        const uint64_t kPNGSignature = 0x0a1a0a0d474e5089ULL;

        struct ChunkIHDR
        {
            uint32_t width;
            uint32_t height;
            uint8_t bit_depth;
            uint8_t color_type;
            uint8_t compresison_method;
            uint8_t filter_method;
            uint8_t interlace_method;
        };

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
            ChunkIHDR chunk_ihdr_;
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_H_