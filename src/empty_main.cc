#include <iostream>
#include <fstream>
#include <cstdint>

#include "color_grayscale.h"
#include "image_png_chunk.h"

#define CRCPP_USE_CPP11
#include "CRC.h"

#include "zlib.h"

int main(int, char *[])
{
    // const char myHelloString[] = {'H', 'E', 'L', 'L', 'O', ' '};
    // const char myWorldString[] = {'W', 'O', 'R', 'L', 'D'};

    CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());

    std::uint32_t crc;

    // crc = CRC::Calculate(myHelloString, sizeof(myHelloString), table);
    // crc = CRC::Calculate(myWorldString, sizeof(myWorldString), table, crc);

    // //std::cout << std::hex << crc << std::endl;

    paint::image_png::ChunkIHDRData ihdr{__builtin_bswap32(0x48),
                                         __builtin_bswap32(0x30),
                                         0x8U,
                                         paint::image_png::ColorType::kTruecolour,
                                         0x0U,
                                         0x0U,
                                         0x0U};

    // uint64_t true_crc = __builtin_bswap32(0xbcabfe3d);

    std::ifstream file;

    file.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);

    file.open("../../images/w3c_home.png", std::ios_base::binary);
    //file.seekg(8, std::ios_base::beg);
    file.seekg(8);

    uint32_t length = 0;
    file.read(reinterpret_cast<char *>(&length), sizeof(length));

    uint32_t type = 0;
    file.read(reinterpret_cast<char *>(&type), sizeof(type));

    crc = CRC::Calculate(&type, sizeof(type), table);
    crc = CRC::Calculate(&ihdr, sizeof(ihdr), table, crc);
    std::cout << std::hex << crc << std::endl;

    uLong crcZ = crc32(0L, Z_NULL, 0);

    crcZ = crc32(crcZ, reinterpret_cast<const unsigned char *>(&type), 4);
    crcZ = crc32(crcZ, reinterpret_cast<const unsigned char *>(&ihdr), sizeof(ihdr));

    length = __builtin_bswap32(length);
    type = __builtin_bswap32(type);

    std::cout << std::hex << crcZ << std::endl;
}