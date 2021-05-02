#include "image_png_chunk.h"

#define CRCPP_USE_CPP11
#include "CRC.h"

namespace paint
{
    namespace image_png
    {
        const CRC::Table<std::uint32_t, 32> crc32_table(CRC::CRC_32());
    }
}