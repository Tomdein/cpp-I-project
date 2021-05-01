#include <iostream>
#include <fstream>

#include "color_grayscale.h"
#include "image_png_chunk.h"

#define CRCPP_USE_CPP11
#include "CRC.h"

#include "zlib.h"

int main(int, char *[])
{
    // const char myHelloString[] = {'H', 'E', 'L', 'L', 'O', ' '};
    // const char myWorldString[] = {'W', 'O', 'R', 'L', 'D'};

    // CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());

    // std::uint32_t crc;

    // crc = CRC::Calculate(myHelloString, sizeof(myHelloString), table);
    // crc = CRC::Calculate(myWorldString, sizeof(myWorldString), table, crc);

    // //std::cout << std::hex << crc << std::endl;

    // paint::image_png::ChunkIHDRData ihdr{0x48, 0x30, 0x8, paint::image_png::ColorType::Truecolour, 0x0, 0x0, 0x0};
    // uint64_t true_crc = 0xbcabfe3d;

    // crc = CRC::Calculate(&ihdr, sizeof(ihdr), table);

    // std::cout << std::hex << crc << std::endl;

    uLong crc = crc32(0L, Z_NULL, 0);

    crc = crc32(crc, buffer, length);

    std::cout << std::hex << crc << std::endl;
}