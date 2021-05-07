#ifndef PAINT_INC_IMAGE_PNG_CHUNK_H_
#define PAINT_INC_IMAGE_PNG_CHUNK_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cassert>

#define CRCPP_USE_CPP11
#include "CRC.h"

#include "zlib.h"

namespace paint
{
    namespace image_png
    {
        /**
         * @brief Signature of the file signifying it is a PNG picture.
         * 
         */
        const uint64_t kPNGSignature = 0x0a1a0a0d474e5089ULL;

        extern const CRC::Table<std::uint32_t, 32> crc32_table;

        enum ChunkType : uint32_t
        {
            kChunkIHDR = 0x52444849U,
            kChunkPLTE = 0x45544c50U,
            kChunkIDAT = 0x54414449U,
            kChunkIEND = 0x444e4549U,
        };

        enum ColorType : uint8_t
        {
            kGrayscale = 0,
            kTruecolour = 2,
            kIndexed_colour = 3,
            kGrayscaleWithAlpha = 4,
            kTruecolourWithAlpha = 6
        };

#pragma pack(1)
        struct ChunkIHDRData
        {
            uint32_t width;
            uint32_t height;
            uint8_t bit_depth;
            ColorType color_type;
            uint8_t compresison_method;
            uint8_t filter_method;
            uint8_t interlace_method;
        };
#pragma pack()

        class ChunkPNG
        {
        public:
            ChunkPNG(uint32_t length, uint32_t type, const std::shared_ptr<uint8_t[]> &data, uint32_t crc, bool little_endian = false)
                : length_(length),
                  type_(type),
                  data_(data),
                  crc_(crc),
                  little_endian_(little_endian) {}

            ChunkPNG(const ChunkPNG &other) = delete;
            ChunkPNG &operator=(const ChunkPNG &other) = delete;

            virtual std::shared_ptr<ChunkPNG> clone() const
            {
                uint32_t len = length_;

                // If length is in big endian -> transform to little endian to get size of data
                if (!little_endian_)
                    len = __builtin_bswap32(len);

                // Create data buffer
                std::shared_ptr<uint8_t[]> new_data = std::make_shared<uint8_t[]>(len);

                // Copy data
                std::copy_n(data_.get(), len, new_data.get());

                return std::make_shared<ChunkPNG>(length_, type_, new_data, crc_, little_endian_);
            }

            bool ValidCRC();

            uint32_t GetLength() const { return length_; }
            uint32_t GetType() const { return type_; }
            std::weak_ptr<const uint8_t[]> GetData() const { return data_; }
            uint32_t GetCRC() const { return crc_; }

            bool IsLittleEndian() const { return little_endian_; }

            bool SafeToCopy()
            {
                uint32_t type = (IsLittleEndian()) ? type_ : __builtin_bswap32(type_);
                return type & (0b00010000 << 24); // 5th bit of 4th byte is the 'Safe-to-copy bit' (5.4 Chunk naming conventions in 'PNG spec 2.0')
            }

            virtual void ToLittleEndian()
            {
                if (!little_endian_)
                {
                    length_ = __builtin_bswap32(length_);
                    type_ = __builtin_bswap32(type_);
                    crc_ = __builtin_bswap32(crc_);
                }
            }

            virtual void ToBigEndian()
            {
                if (little_endian_)
                {
                    length_ = __builtin_bswap32(length_);
                    type_ = __builtin_bswap32(type_);
                    crc_ = __builtin_bswap32(crc_);
                }
            }

            void RecalculateCRC()
            {
                // CRC is calculated on big endian data
                assert(!little_endian_);

                crc_ = CRC::Calculate(&type_, sizeof(type_), crc32_table);
                crc_ = CRC::Calculate(&data_, length_, crc32_table, crc_);
            }

        protected:
            uint32_t length_;                       /// The length of the PNG chunk (only the size of data_, not type_ or crc_).
            uint32_t type_;                         /// Type of the PNG chunk (4 bytes defining the chunk type).
            const std::shared_ptr<uint8_t[]> data_; /// The data of the PNG chunk (can be 0).
            uint32_t crc_;                          /// 4 byte CRC calculated from preceding type_ and data_ (NOT length_).

            bool little_endian_ = false;
        };

        class ChunkIHDR : public ChunkPNG
        {
        public:
            ChunkIHDR(const std::shared_ptr<ChunkIHDRData> &data, uint32_t crc, bool little_endian = false)
                : ChunkPNG(sizeof(ChunkIHDRData), 0x52444849U, std::reinterpret_pointer_cast<uint8_t[]>(data), crc, little_endian) {}

            ChunkIHDR(uint32_t length, const std::shared_ptr<uint8_t[]> &data, uint32_t crc, bool little_endian = false)
                : ChunkPNG(length, 0x52444849U, data, crc, little_endian) {}

            ChunkIHDR(const ChunkIHDR &other) = delete;
            ChunkIHDR &operator=(const ChunkIHDR &other) = delete;

            virtual std::shared_ptr<ChunkPNG> clone() const override
            {
                uint32_t len = length_;

                // If length is in big endian -> transform to little endian to get size of data
                if (!little_endian_)
                    len = __builtin_bswap32(len);

                // Create data buffer
                std::shared_ptr<uint8_t[]> new_data = std::make_shared<uint8_t[]>(len);

                // Copy data
                std::copy_n(data_.get(), len, new_data.get());

                return std::make_shared<ChunkIHDR>(length_, new_data, crc_, little_endian_);
            }

            virtual void ToLittleEndian() override
            {
                if (!little_endian_)
                {
                    ChunkPNG::ToLittleEndian();
                    SwapIHDRDataEndianity();
                }
            }

            virtual void ToBigEndian() override
            {
                if (little_endian_)
                {
                    ChunkPNG::ToBigEndian();
                    SwapIHDRDataEndianity();
                }
            }

            std::weak_ptr<ChunkIHDRData> GetIHDRData() { return std::reinterpret_pointer_cast<ChunkIHDRData>(data_); }

        private:
            void SwapIHDRDataEndianity()
            {
                // Get data
                auto wptr_data = GetIHDRData();
                auto sptr_data = wptr_data.lock();

                // No data in shared_ptr
                if (!sptr_data)
                {
                    std::cerr << "Cannot convert ChunkIHDR endianity - Data missing.";
                    throw "Cannot convert ChunkIHDR endianity - Data missing.";
                }

                // Swap endianity
                sptr_data->width = __builtin_bswap32(sptr_data->width);
                sptr_data->height = __builtin_bswap32(sptr_data->height);
            }
        };

        class ChunkPLTE : public ChunkPNG
        {
        public:
            ChunkPLTE(uint32_t length, const std::shared_ptr<uint8_t[]> &data, uint32_t crc, bool little_endian = false)
                : ChunkPNG(length, 0x45544c50U, data, crc, little_endian) {}

            ChunkPLTE(const ChunkPLTE &other) = delete;
            ChunkPLTE &operator=(const ChunkPLTE &other) = delete;

            virtual std::shared_ptr<ChunkPNG> clone() const override
            {
                uint32_t len = length_;

                // If length is in big endian -> transform to little endian to get size of data
                if (!little_endian_)
                    len = __builtin_bswap32(len);

                // Create data buffer
                std::shared_ptr<uint8_t[]> new_data = std::make_shared<uint8_t[]>(len);

                // Copy data
                std::copy_n(data_.get(), len, new_data.get());

                return std::make_shared<ChunkPLTE>(length_, new_data, crc_, little_endian_);
            }

            virtual void ToLittleEndian() override
            {
                if (!little_endian_)
                {
                    ChunkPNG::ToLittleEndian();

                    // TODO transform IHDRData to little endian
                }
            }

            virtual void ToBigEndian() override
            {
                if (little_endian_)
                {
                    ChunkPNG::ToBigEndian();

                    // TODO transform IHDRData to big endian
                }
            }
        };

        class ChunkIDAT : public ChunkPNG
        {
        public:
            ChunkIDAT(uint32_t length, const std::shared_ptr<uint8_t[]> &data, uint32_t crc, bool little_endian = false, bool deflated = true)
                : ChunkPNG(length, 0x54414449U, data, crc, little_endian), deflated_(deflated) {}

            virtual std::shared_ptr<ChunkPNG> clone() const override
            {
                uint32_t len = length_;

                // If length is in big endian -> transform to little endian to get size of data
                if (!little_endian_)
                    len = __builtin_bswap32(len);

                // Create data buffer
                std::shared_ptr<uint8_t[]> new_data = std::make_shared<uint8_t[]>(len);

                // Copy data
                std::copy_n(data_.get(), len, new_data.get());

                return std::make_shared<ChunkIDAT>(length_, new_data, crc_, little_endian_);
            }

            ChunkIDAT(const ChunkIDAT &other) = delete;
            ChunkIDAT &operator=(const ChunkIDAT &other) = delete;

            virtual void ToLittleEndian() override
            {
                if (!little_endian_)
                {
                    ChunkPNG::ToLittleEndian();

                    // TODO transform IHDRData to little endian
                }
            }

            virtual void ToBigEndian() override
            {
                if (little_endian_)
                {
                    ChunkPNG::ToBigEndian();

                    // TODO transform IHDRData to big endian
                }
            }

            /**
             * @brief Set the size of single pixel. 
             * 
             * Sets the size of one pixel in bytes -> used when changing endianity.
             * Set from bit depth in IHDR chunk.
             * 
             * Only private variable.
             * 
             * @param size of pixel in bytes.
             */
            void SetPixelSizeBytes(size_t size) { pixel_size_bytes_ = size; }

            /**
             * @brief Return wether data is compressed (deflated) or not.
             * 
             * Used to find if data is compressed using Inflate/Deflate algorithm.
             * 
             * @return true if data is deflated (decompressed).
             * @return false if data is inflated (compressed).
             */
            bool IsDeflated() { return deflated_; }
            /**
             * @brief Deflate the data.
             * 
             * Deflate (compress) the data.
             * 
             * @param zstream used for deflating.
             */
            void Deflate(z_stream &zstream, const std::unique_ptr<uint8_t[]> data_to_deflate)
            {
                // Data already compressed
                if (deflated_)
                    return;

                int error;

                zstream.avail_out = (little_endian_) ? length_ : __builtin_bswap32(length_);
                zstream.next_out = data_to_deflate.get();
                do
                {
                    // Should never run out of space
                    assert(zstream.avail_out);

                    error = deflate(&zstream, Z_NO_FLUSH);
                    if (error != Z_OK && error != Z_STREAM_END)
                    {
                        std::cerr << "Error while deflating IDAT chunk!" << std::endl;
                        throw "Error while deflating IDAT chunk";
                    }
                } while (error != Z_STREAM_END);

                deflated_ = true;
            }
            /**
             * @brief Inflate the data.
             * 
             * Inflate (decompress) the data.
             * 
             * @param zstream used for inflating.
             */
            void Inflate(z_stream &zstream)
            {
                // Data already decompressed
                if (!deflated_)
                    return;

                int error;
                zstream.avail_in = (little_endian_) ? length_ : __builtin_bswap32(length_);
                zstream.next_in = data_.get();
                do
                {
                    // Should never run out of space
                    assert(zstream.avail_out);

                    error = inflate(&zstream, Z_NO_FLUSH);
                    if (error != Z_OK && error != Z_STREAM_END)
                    {

                        std::cerr << "Error while inflating IDAT chunk!" << std::endl;
                        throw "Error while inflating IDAT chunk";
                    }
                } while (error != Z_STREAM_END);

                deflated_ = false;
            }

        private:
            size_t pixel_size_bytes_ = 1; /// Size of pixel in bytes.
            bool deflated_ = true;        /// Data is deflated (compressed).
        };

        class ChunkIEND : public ChunkPNG
        {
        public:
            ChunkIEND(uint32_t crc, bool little_endian = false)
                : ChunkPNG(0, 0x444e4549U, std::shared_ptr<uint8_t[]>(), crc, little_endian) {}

            virtual std::shared_ptr<ChunkPNG> clone() const override
            {
                uint32_t len = length_;

                // If length is in big endian -> transform to little endian to get size of data
                if (!little_endian_)
                    len = __builtin_bswap32(len);

                // Create data buffer
                std::shared_ptr<uint8_t[]> new_data = std::make_shared<uint8_t[]>(len);

                // Copy data
                std::copy_n(data_.get(), len, new_data.get());

                return std::make_shared<ChunkIEND>(crc_, little_endian_);
            }

            ChunkIEND(const ChunkIEND &other) = delete;
            ChunkIEND &operator=(const ChunkIEND &other) = delete;

            virtual void ToLittleEndian() override
            {
                if (!little_endian_)
                {
                    ChunkPNG::ToLittleEndian();
                }
            }

            virtual void ToBigEndian() override
            {
                if (little_endian_)
                {
                    ChunkPNG::ToBigEndian();
                }
            }
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_CHUNK_H_