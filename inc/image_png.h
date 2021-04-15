#ifndef PAINT_INC_IMAGE_PNG_H_
#define PAINT_INC_IMAGE_PNG_H_

#include <cstdint>

#include "image.h"

namespace paint
{
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

    const static uint64_t PNG_SIGNATURE;

    template <class T>
    class ImagePNG : Image<T>
    {
    public:
        ImagePNG() = default;

        virtual void LoadImage() override {}
        virtual void SaveImage() override {}
        virtual void GenerateMetadata() override {}

    private:
        ChunkIHDR chunk_ihdr_;
    };
}

#endif // PAINT_INC_IMAGE_PNG_H_