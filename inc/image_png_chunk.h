#ifndef PAINT_INC_IMAGE_PNG_CHUNK_H_
#define PAINT_INC_IMAGE_PNG_CHUNK_H_

#include <cstdint>
#include <memory>

namespace paint
{
    namespace image_png
    {
        const uint64_t kChunkIHDR = 0x52444849U;
        const uint64_t kChunkPLTE = 0x45544c50U;
        const uint64_t kChunkIDAT = 0x54414449U;
        const uint64_t kChunkIEND = 0x444e4549U;

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

        struct ChunkPLTEData
        {
        };
#pragma pack()

        class ChunkPNG
        {
        public:
            ChunkPNG(uint64_t length, uint64_t type, const std::shared_ptr<uint8_t[]> &data, uint64_t crc) : length_(length),
                                                                                                             type_(type),
                                                                                                             data_(data),
                                                                                                             crc_(crc) {}

            bool ValidCRC();

            uint64_t GetLength() { return length_; }
            uint64_t GetType() { return type_; }
            std::weak_ptr<const uint8_t[]> GetData() { return data_; }
            uint64_t GetCRC() { return crc_; }

        protected:
            uint64_t length_;                       /// The length of the PNG chunk (only the size of data_, not type_ or crc_).
            uint64_t type_;                         /// Type of the PNG chunk (4 bytes defining the chunk type).
            const std::shared_ptr<uint8_t[]> data_; /// The data of the PNG chunk (can be 0).
            uint64_t crc_;                          /// 4 byte CRC calculated from preceding type_ and data_ (NOT length_).
        };

        class ChunkIHDR : public ChunkPNG
        {
        public:
            ChunkIHDR(uint64_t length, const std::shared_ptr<uint8_t[]> &data, uint64_t crc) : ChunkPNG(length, 0x52444849U, data, crc) {}

            std::weak_ptr<ChunkIHDRData> GetIHDRData() { return std::reinterpret_pointer_cast<ChunkIHDRData>(data_); }
        };

        class ChunkPLTE : public ChunkPNG
        {
        public:
            ChunkPLTE(uint64_t length, const std::shared_ptr<uint8_t[]> &data, uint64_t crc) : ChunkPNG(length, 0x45544c50U, data, crc) {}
        };

        class ChunkIDAT : public ChunkPNG
        {
        public:
            ChunkIDAT(uint64_t length, const std::shared_ptr<uint8_t[]> &data, uint64_t crc) : ChunkPNG(length, 0x54414449U, data, crc) {}
        };

        class ChunkIEND : public ChunkPNG
        {
        public:
            ChunkIEND(uint64_t crc) : ChunkPNG(0, 0x444e4549U, std::shared_ptr<uint8_t[]>(), crc) {}
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_CHUNK_H_