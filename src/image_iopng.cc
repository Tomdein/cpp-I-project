#include "image_iopng.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <exception>

#define CRCPP_USE_CPP11
#include "CRC.h"

#include "image_png.h"
#include "color_rgb888.h"
#include "data_pixels.h"

namespace paint
{
    namespace image_png
    {
        void ImageIOPNG::ReadPNGFile(std::ifstream &file, ImagePNG &img)
        {

            std::shared_ptr<ChunkPNG> chunk;

            chunk = ImageIOPNG::ReadChunkPNG(file);

            // Get chunk type
            uint32_t type = chunk->GetType();
            if (!chunk->IsLittleEndian())
                type = __builtin_bswap32(type);

            // First chunk must be IHDR chunk:
            if (type != ChunkType::kChunkIHDR)
            {
                std::cerr << "First chunk must be IHDR chunk!" << std::endl;
                throw error_chunk_mismatch();
            }

            img.chunk_ihdr_ = std::reinterpret_pointer_cast<ChunkIHDR>(chunk);

            // TODO: Read all chunks up to IDAT, if any of them is PLTE -> save and set SetPixelSizeBytes(), otherwise push to vec_chunks or ?discard?
            while (true)
            {
                // READ CHUNK
                chunk = ImageIOPNG::ReadChunkPNG(file);

                // Get chunk type
                uint32_t type = chunk->GetType();
                if (!chunk->IsLittleEndian())
                    type = __builtin_bswap32(type);

                // IS PLTE CHUNK
                if (type == ChunkType::kChunkPLTE)
                {
                    // If PNG already has read PLTE chunk -> throw
                    if (img.chunk_plte_.get())
                    {
                        std::cerr << "PLTE chunk already exists!" << std::endl;
                        throw error_chunk_mismatch();
                    }

                    img.chunk_plte_ = std::reinterpret_pointer_cast<ChunkPLTE>(chunk);
                }

                // IS IEND CHUNK
                // No IDAT chunk or IEND chunk is befor IDAT chunk -> error
                else if (type == ChunkType::kChunkIEND)
                {
                    std::cerr << "No IDAT chunk or IEND chunk is before IDAT chunk!" << std::endl;
                    throw error_chunk_mismatch();
                }

                // IS IDAT CHUNK
                // Read unill IDAT chunk
                else if (type == ChunkType::kChunkIDAT)
                    break;

                // IS ANCILLARY CHUNK
                // If not yet read PLTE chunk -> add ancillary chunk to vec_chunk_pre_plte_ otherwise add to vec_chunk_pre_idat_
                if (!img.chunk_plte_.get())
                {
                    img.vec_chunk_pre_plte_.emplace_back(std::move(chunk));
                }
                else
                {
                    img.vec_chunk_pre_idat_.emplace_back(std::move(chunk));
                }
            }

            // TODO: read IDAT chunks and possibly ANCILLARY chunks
            std::cerr << "TODO: void ImagePNG::LoadImage()" << std::endl;
            throw;

            while (true)
            {

                // READ CHUNK
                chunk = ImageIOPNG::ReadChunkPNG(file);

                // Get chunk type
                uint32_t type = chunk->GetType();
                if (!chunk->IsLittleEndian())
                    type = __builtin_bswap32(type);

                // IS IEND CHUNK
                if (type == ChunkType::kChunkIEND)
                {
                    break;
                }

                img.vec_chunk_pre_none.emplace_back(std::move(chunk));
            }

            // Set IEND chunk
            img.chunk_iend_ = std::reinterpret_pointer_cast<ChunkIEND>(chunk);

            img.image_loaded_ = true;

            // Last chunk must be IEND chunk
            // DONE READING PNG
        }

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

        void ImageIOPNG::ReadIDATChunks(ImagePNG &img)
        {
            // Do nothing if no image is loaded
            if (!img.image_loaded_)
            {
                std::cerr << "ImagePNG::ReadIDATChunks - No image is loaded" << std::endl;
                return;
            }

            if (!img.chunk_ihdr_->IsLittleEndian())
                img.chunk_ihdr_->ToLittleEndian();

            auto wptr_ihdr_data = img.chunk_ihdr_->GetIHDRData();

            if (wptr_ihdr_data.expired())
            {
                std::cerr << "Shared ptr to IHDRData is expired!" << std::endl;
                assert(!wptr_ihdr_data.expired());
            }

            auto sptr_ihdr_data = wptr_ihdr_data.lock();

            auto width = sptr_ihdr_data->width;
            auto height = sptr_ihdr_data->height;
            auto bit_depth = sptr_ihdr_data->bit_depth;

            // -- START INFLATING
            // +7 because integer division is used
            // +height because each scanline has filtering byte
            auto size = (width * height * bit_depth + 7) / 8 + height;
            auto sptr_inflated_data = std::make_shared<uint8_t[]>(size);

            z_stream zstream;
            auto error = inflateInit(&zstream);
            zstream.avail_out = size;
            zstream.next_out = sptr_inflated_data.get();

            if (error != Z_OK)
            {
                std::cerr << "ImagePNG::ReadIDATChunks - Error occured when creating zstream (zlib)!" << std::endl;
                throw "ImagePNG::ReadIDATChunks - Error occured when creating zstream (zlib)";
            }

            // Inflate each IDAT chunk into buffer
            std::for_each(img.vec_chunk_data_.begin(), img.vec_chunk_data_.end(), [&zstream](auto &idat_chunk) {
                idat_chunk->Inflate(zstream);
                return;
            });

            inflateEnd(&zstream);
            // -- DONE INFLATING

            // -- START FILTERING
            // Filter buffer scanline by scanline
            //TODO: Filter inflated scanlines (+ 1byte filtering at the start of each scanline).
            for (uint32_t y = 0; y < height; y++)
            {
                switch (sptr_inflated_data[y * width])
                {

                // // Filter type 1 - 'None'
                case 0:
                    break;

                // Filter type 1 - 'Sub'
                case 1:
                    // Add 0 to first byte (does not have any byte to the left) -> do nothing with first byte
                    // Start from second byte (1byte filtering + first byte has no left byte)
                    for (uint32_t x = 2; x <= width; x++)
                    {
                        // Intentional underflow/overflow (filter is modulo 256)
                        sptr_inflated_data[y * width + x] += sptr_inflated_data[y * width + x - 1];
                    }
                    break;

                // Filter type 2 - 'Up'
                case 2:
                    // First scanline does nothing - has no line above -> start from second scanline
                    if (y == 0)
                        break;

                    for (uint32_t x = 1; x <= width; x++)
                    {
                        // Intentional underflow/overflow (filter is modulo 256)
                        sptr_inflated_data[y * width + x] += sptr_inflated_data[(y - 1) * width + x];
                    }
                    break;

                // Filter type 3 - 'Average'
                case 3:
                    // First scanline does nothing - has no line above -> start from second scanline
                    if (y == 0)
                        break;

                    // Add only above byte to first byte (does not have any byte to the left)
                    sptr_inflated_data[y * width + 1] += sptr_inflated_data[(y - 1) * width + 1];

                    // Start for loop from second byte (1byte filtering + first byte has no left byte)
                    for (uint32_t x = 2; x <= width; x++)
                    {
                        // Intentional underflow/overflow (filter is modulo 256)
                        sptr_inflated_data[y * width + x] += sptr_inflated_data[y * width + x - 1] + sptr_inflated_data[(y - 1) * width + x];
                    }
                    break;

                // Filter type 4 - 'Paeth'
                case 4:
                    //TODO: Optimize filter type 4
                    // 9.4 Filter type 4: Paeth (https://www.w3.org/TR/2003/REC-PNG-20031110/#9Filters)
                    for (uint32_t x = 1; x <= width; x++)
                    {
                        auto a = 0;
                        auto b = 0;
                        auto c = 0;

                        // a is the left byte
                        if (x != 1)
                            a = sptr_inflated_data[y * width + x - 1];

                        // b is top byte
                        if (y != 0)
                            b = sptr_inflated_data[(y - 1) * width + x];

                        // c is top left byte
                        if (x != 1 && y != 0)
                            b = sptr_inflated_data[(y - 1) * width + x];

                        auto p = a + b - c;
                        auto pa = std::abs(p - a);
                        auto pb = std::abs(p - b);
                        auto pc = std::abs(p - c);

                        if (pa <= pb && pa <= pc)
                        {
                            sptr_inflated_data[y * width + x] = pa;
                        }
                        else if (pb <= pc)
                        {
                            sptr_inflated_data[y * width + x] = pb;
                        }
                        else
                        {
                            sptr_inflated_data[y * width + x] = pc;
                        }
                    }
                    break;
                }
            }
            // -- DONE FILTERING

            // -- START SAVE BUFFER TO PIXEL DATA
            //TODO: Add support to other colors
            if (sptr_ihdr_data->bit_depth != 8)
            {
                std::cerr << "Unsupported PNG bit depth" << sptr_ihdr_data->bit_depth << std::endl;
                throw "Unsupported PNG bit depth";
            }

            if (sptr_ihdr_data->color_type != 2)
            {
                std::cerr << "Unsupported PNG color type" << sptr_ihdr_data->color_type << std::endl;
                throw "Unsupported PNG color type";
            }

            std::unique_ptr<Color> color(new ColorRGB888());

            img.image_data_ = std::make_shared<DataPixels>(Point{static_cast<int>(width), static_cast<int>(height)}, std::move(color));

            for (uint32_t y = 0; y < height; y++)
            {
                for (uint32_t x = 1; x <= width; x++)
                {
                    // RGB888 only
                    std::copy_n(&sptr_inflated_data[y * width + (x - 1) * 3], 3, reinterpret_cast<uint8_t *>((*img.image_data_)[y * width + x - 1]));
                }
            }
            // -- DONE SAVE BUFFER TO PIXEL DATA
        }

        void ImageIOPNG::WriteChunkPNG(std::ofstream &file, std::shared_ptr<ChunkPNG> chunk)
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

        void ImageIOPNG::GenerateIHDRChunks(ImagePNG &img)
        {
            auto ihdr_data = std::make_shared<ChunkIHDRData>();
            ihdr_data->width = img.image_data_->GetSize().x;
            ihdr_data->height = img.image_data_->GetSize().y;
            ihdr_data->bit_depth = img.image_data_->GetColorType()->GetDataSizeBits() / 3;
            //TODO: support different color types
            ihdr_data->color_type = ColorType::kTruecolour;
            ihdr_data->compresison_method = 0;
            ihdr_data->filter_method = 0;
            //TODO: add interlacing support
            ihdr_data->interlace_method = 0;

            auto chunk_ihdr = std::make_shared<ChunkIHDR>(ihdr_data, 0, true);
            chunk_ihdr->ToBigEndian();
            chunk_ihdr->RecalculateCRC();
        }

        void ImageIOPNG::GenerateIDATChunks(ImagePNG &img)
        {
            auto pixel_data = img.image_data_;
            auto width = pixel_data->GetSize().x;
            auto height = pixel_data->GetSize().y;

            //TODO: support for 1/2/4 bit per channel (filtering is not recommended)
            auto pixel_size_bytes = pixel_data->GetColorType()->GetDataSize();

            // Width + 1 -> 1st byte is filter byte
            auto idat_buffer = std::make_unique<uint8_t[]>((width + 1) * height * pixel_size_bytes);

            // Original previous scanline for Average and Paeth firlters
            auto scanline_previous = std::make_unique<uint8_t[]>((width + 1) * pixel_size_bytes);

            // Scanline buffers used to figure the best filter type for scanline
            auto scanline_buffer1 = std::make_unique<int[]>((width + 1) * pixel_size_bytes);
            auto scanline_buffer2 = std::make_unique<int[]>((width + 1) * pixel_size_bytes);

            for (int y = 0; y < height; y++)
            {
                if (y > 0)
                    std::copy_n(reinterpret_cast<uint8_t *>((*pixel_data)[(y - 1) * width]), width * pixel_size_bytes, scanline_previous.get());

                // Filter type 0 - 'None'
                uint8_t filter_type = 0;
                std::copy_n(reinterpret_cast<uint8_t *>((*pixel_data)[y * width]), width * pixel_size_bytes, scanline_buffer1.get());

                // Filter type 1 - 'Sub'
                std::copy_n(reinterpret_cast<uint8_t *>((*pixel_data)[y * width]), width * pixel_size_bytes, scanline_buffer2.get());
                for (int x = width * pixel_size_bytes - 1; x > 0; x++)
                {
                    scanline_buffer2[x] -= scanline_buffer2[x - 1];
                }

                // If filter type 1 is better choose it
                if (ValuesInScanline(scanline_buffer1, width * pixel_size_bytes) > ValuesInScanline(scanline_buffer2, width * pixel_size_bytes))
                {
                    std::swap(scanline_buffer1, scanline_buffer2);
                    filter_type = 1;
                }

                // Filter type 2 - 'Up'
                std::copy_n(reinterpret_cast<uint8_t *>((*pixel_data)[y * width]), width * pixel_size_bytes, scanline_buffer2.get());
                for (int x = width * pixel_size_bytes - 1; x >= 0; x++)
                {
                    // No upper scanline -> skip
                    if (y == 0)
                        break;

                    scanline_buffer2[x] -= scanline_previous[x];
                }

                // If filter type 2 is better choose it
                if (ValuesInScanline(scanline_buffer1, width * pixel_size_bytes) > ValuesInScanline(scanline_buffer2, width * pixel_size_bytes))
                {
                    std::swap(scanline_buffer1, scanline_buffer2);
                    filter_type = 2;
                }

                // Filter type 3 - 'Average'
                std::copy_n(reinterpret_cast<uint8_t *>((*pixel_data)[y * width]), width * pixel_size_bytes, scanline_buffer2.get());
                for (int x = width * pixel_size_bytes - 1; x > 0; x--)
                {
                    // No upper scanline -> skip
                    if (y == 0)
                        break;

                    scanline_buffer2[x] -= (scanline_buffer2[x] + scanline_previous[x]) / 2;
                }

                // If filter type 3 is better choose it
                if (ValuesInScanline(scanline_buffer1, width * pixel_size_bytes) > ValuesInScanline(scanline_buffer2, width * pixel_size_bytes))
                {
                    std::swap(scanline_buffer1, scanline_buffer2);
                    filter_type = 3;
                }

                //TODO: Test Paeth filter

                // Set filter byte
                idat_buffer[y * width] = filter_type;
                // Copy scanline
                std::copy_n(scanline_buffer2.get(), width * pixel_size_bytes, &idat_buffer[y * width + 1]);
            }

            // -- START DEFLATING
            // +7 because integer division is used
            // +height because each scanline has filtering byte
            auto size = (width + 1) * height * pixel_size_bytes;
            auto sptr_deflated_data = std::make_shared<uint8_t[]>(size + 10);

            z_stream zstream;
            auto error = deflateInit(&zstream, Z_DEFAULT_COMPRESSION);
            zstream.avail_in = size;
            zstream.next_in = idat_buffer.get();
            zstream.avail_out = size + 10;
            zstream.next_out = sptr_deflated_data.get();

            if (error != Z_OK)
            {
                std::cerr << "ImagePNG::GenerateIDATChunks - Error occured when creating zstream (zlib)!" << std::endl;
                throw "ImagePNG::GenerateIDATChunks - Error occured when creating zstream (zlib)";
            }

            error = deflate(&zstream, Z_NO_FLUSH);
            auto deflated_size = size + 10 - zstream.avail_out;

            if (error != Z_STREAM_END)
            {
                std::cerr << "Error while deflating IDAT chunk! (data remaining)" << std::endl;
                throw "Error while deflating IDAT chunk (data remaining)";
            }

            deflateEnd(&zstream);
            // -- DONE DEFLATING

            //TODO: deflate into multiple IDAT chunks
            img.vec_chunk_data_.resize(1);
            auto idat_chunk = std::make_shared<ChunkIDAT>(deflated_size, sptr_deflated_data, 0, true);
            idat_chunk->ToLittleEndian();
            idat_chunk->RecalculateCRC();
            img.vec_chunk_data_[0] = idat_chunk;
        }

        unsigned int ImageIOPNG::ValuesInScanline(const std::unique_ptr<int[]> &scanline, int bytes)
        {
            return std::accumulate(scanline.get(), scanline.get() + bytes, 0, [](auto &v1, auto &v2) {
                return std::abs(v1) + std::abs(v2);
            });
        }
    }
}