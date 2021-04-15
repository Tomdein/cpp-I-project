#ifndef PAINT_INC_IMAGE_BPM_H_
#define PAINT_INC_IMAGE_BPM_H_

#include "image.h"

namespace paint
{
    namespace image_bmp
    {

        const uint16_t kBfType = 0x424dU;

        /**
         * @brief Number of bit planes. Always 1 (historic reason)
         * 
         */
        const uint16_t kPlanes = 1;

        /**
         * @brief Number of bits per pixel
         * 
         */
        enum BiBitCount
        {
            k1bpPX = 1,
            k4bpPX = 4,
            k8bpPX = 8,
            k24bpPX = 24,
        };

        /**
         * @brief Comprimation method used
         * 
         */
        enum BiCompression
        {
            kBiRGB = 0,
            kBiRLE8 = 1,
            kBiRLE4 = 2,
        };
        struct HeaderBMP
        {
            uint16_t bf_type;
            uint32_t bf_size;
            uint16_t bf_reserved1;
            uint16_t bf_reserved2;
            uint32_t bf_offBits;
        };

        struct HeaderBMPInfo
        {
            uint32_t bi_size;
            uint32_t bi_widht;
            uint32_t bi_height;
            uint16_t bi_planes;
            uint16_t bi_bitCount;
            uint32_t bi_compression;
            uint32_t bi_sizeImage;
            uint32_t bi_xPelsPerMeter;
            uint32_t bi_yPelsPerMeter;
            uint32_t bi_clrUsed;
            uint32_t bi_clrImportant;
        };

        class ImageBMP : public Image
        {
        public:
            virtual void LoadImage() override {}
            virtual void SaveImage() override {}
            virtual void GenerateMetadata() override {}

        private:
            HeaderBMP header_bmp_;
            HeaderBMPInfo header_bmp_info_;

            bool CheckHeader();
            bool CheckHeaderInfo();
        };
    }
}

#endif // PAINT_INC_IMAGE_BPM_H_