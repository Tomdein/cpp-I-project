#include "image_iopng.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>

#define CRCPP_USE_CPP11
#include "CRC.h"

namespace paint
{
    namespace image_png
    {
        std::shared_ptr<ChunkPNG> ImageIOPNG::ReadChunkPNG(std::ifstream &file)
        {
            uint32_t length;
            file.read(reinterpret_cast<char *>(&length), sizeof(length));
            uint32_t length_data = __builtin_bswap32(length);

            uint32_t type;
            file.read(reinterpret_cast<char *>(&type), sizeof(type));

            std::shared_ptr<uint8_t[]> data(new uint8_t[length_data]);
            file.read(reinterpret_cast<char *>(data.get()), length_data);

            uint32_t crc;
            file.read(reinterpret_cast<char *>(&crc), sizeof(crc));

            // std::transform(data, data.get() + length, std::back_inserter(vec_data), [auto x]() { return __builtin_bswap64 })

            CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());

            std::uint32_t crc_calculated;
            crc_calculated = CRC::Calculate(&type, sizeof(type), table);
            crc_calculated = CRC::Calculate(data.get(), sizeof(length_data), table, crc);

            if (crc_calculated != __builtin_bswap32(crc))
            {
                std::cerr << "CRC chechsums does not match!!!" << std::endl;
                throw "CRC chechsums does not match";
            }

            std::shared_ptr<ChunkPNG> chunk;

            switch (type)
            {
            // Type == 'IHDR' in little endian
            case ChunkType::kChunkIHDR:
                chunk = std::shared_ptr<ChunkPNG>(new ChunkIHDR(length, data, crc));
                break;

            // Type == 'PLTE' in little endian
            case ChunkType::kChunkPLTE:
                chunk = std::shared_ptr<ChunkPNG>(new ChunkPLTE(length, data, crc));
                break;

            // Type == 'IDAT' in little endian
            case ChunkType::kChunkIDAT:
                chunk = std::shared_ptr<ChunkPNG>(new ChunkIDAT(length, data, crc));
                break;

            // Type == 'IEND' in little endian
            case ChunkType::kChunkIEND:
                chunk = std::shared_ptr<ChunkPNG>(new ChunkIEND(crc));
                break;

            // Unsupported chunks
            default:
                chunk = std::shared_ptr<ChunkPNG>(new ChunkPNG(length, type, data, crc));
            }

            return chunk;
        }

        void ImageIOPNG::WriteChunkPNG(std::ofstream &file, std::shared_ptr<ChunkPNG> &chunk)
        {
            // CLONE the chunk AND transform to big endian -> data does not get edited int the original chunk
            if (chunk->IsLittleEndian())
            {
                chunk = chunk->clone();
                chunk->ToBigEndian();
            }

            // Recalculate CRC
            chunk->RecalculateCRC();

            // Get lenght in little endian
            uint32_t len = __builtin_bswap32(chunk->GetLength());

            // Get data to be written
            uint32_t length = chunk->GetLength();
            uint32_t type = chunk->GetType();
            uint32_t crc = chunk->GetCRC();

            // Write length
            file.write(reinterpret_cast<const char *>(&length), sizeof(length));

            // Write type
            file.write(reinterpret_cast<const char *>(&type), sizeof(type));

            // Write data
            file.write(reinterpret_cast<const char *>(chunk->GetData().lock().get()), len);

            // Write CRC checksum
            file.write(reinterpret_cast<const char *>(&crc), sizeof(crc));
        }
    }
}