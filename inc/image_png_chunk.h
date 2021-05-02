#ifndef PAINT_INC_IMAGE_PNG_CHUNK_H_
#define PAINT_INC_IMAGE_PNG_CHUNK_H_

#include <cstdint>
#include <memory>
#include <algorithm>
#include <cassert>

#define CRCPP_USE_CPP11
#include "CRC.h"

namespace paint
{
    namespace image_png
    {
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
            Grayscale = 0,
            Truecolour = 2,
            Indexed_colour = 3,
            GrayscaleWithAlpha = 4,
            TruecolourWithAlpha = 6
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
            };

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

            std::weak_ptr<ChunkIHDRData> GetIHDRData() { return std::reinterpret_pointer_cast<ChunkIHDRData>(data_); }
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
            ChunkIDAT(uint32_t length, const std::shared_ptr<uint8_t[]> &data, uint32_t crc, bool little_endian = false)
                : ChunkPNG(length, 0x54414449U, data, crc, little_endian) {}

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
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_CHUNK_H_