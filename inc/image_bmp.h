#ifndef PAINT_INC_IMAGE_BPM_H_
#define PAINT_INC_IMAGE_BPM_H_

#include <cstdint>
#include <filesystem>

#include "image.h"
#include "file.h"

namespace paint
{
    namespace image_bmp
    {

        /**
         * @brief 'BM' signifying the file is BMP.
         * 
         * TODO: endianess... little-endian -> 'MB'
         */
        const uint16_t kBfType = 0x4d42U;

        /**
         * @brief Number of bit planes. Always 1 (historic reason).
         * 
         */
        const uint16_t kPlanes = 1;

        /**
         * @brief Number of bits per pixel.
         * 
         */
        enum BiBitCount
        {
            k1bpPX = 1,   /// 1 bit per pixel
            k4bpPX = 4,   /// 4 bits per pixel
            k8bpPX = 8,   /// 8 bits per pixel
            k16bpPX = 16, /// 16 bits per pixel
            k24bpPX = 24, /// 24 bits per pixel
        };

        /**
         * @brief Comprimation method used.
         * 
         */
        enum BiCompression
        {
            kBiRGB = 0,
            kBiRLE8 = 1,
            kBiRLE4 = 2,
        };

// Set the alignment of headers to 1 byte
#pragma pack(1)

        /**
         * @brief Bitmap file header.
         * 
         */
        struct HeaderBMP
        {
            uint16_t bf_type;      /// Identifies the file as BMP ('B','M').
            uint32_t bf_size;      /// Size of BMP file in bytes.
            uint16_t bf_reserved1; /// Reserved.
            uint16_t bf_reserved2; /// Reserved.
            uint32_t bf_offBits;   /// Offset (starting address) of pixel data.
        };

        /**
         * @brief Bitmap info header.
         * 
         */
        struct HeaderBMPInfo
        {
            uint32_t bi_size;          /// Size of this header in bytes.
            uint32_t bi_width;         /// Width of bitmap in pixels.
            uint32_t bi_height;        /// Height of bitmap in pixels.
            uint16_t bi_planes;        /// Number of color planes (MUST be zero).
            uint16_t bi_bitCount;      /// Number of bits per pixel (color depth of the image).
            uint32_t bi_compression;   /// Compression method (see BiCompression).
            uint32_t bi_sizeImage;     /// Size of bitmap data (for BiCompression::kBiRGB can be 0).
            uint32_t bi_xPelsPerMeter; /// Horizontal resolution of the image (ppm - pixels per metre).
            uint32_t bi_yPelsPerMeter; /// Vertical resolution of the image (ppm - pixels per metre).
            uint32_t bi_clrUsed;       /// Number of colors in the color palette (0 as default for 2^n).
            uint32_t bi_clrImportant;  /// The number of important colors used (0 when every color is important). This is generaly ignored.
        };

// Reset to default alignment
#pragma pack()

        /**
         * @brief A class that represents a BMP image.
         * 
         * Each instance of this class represents a separate BMP image.
         * 
         * For editing a image go see class Image.
         * 
         */
        class ImageBMP : public Image
        {
        public:
            ImageBMP(std::filesystem::path input_file_path)
            {
                file_in_ = File{FileType::kBMP, input_file_path};
                file_out_ = File{FileType::kBMP, input_file_path};
            }
            virtual ~ImageBMP() override {}

            /**
             * @brief Create an empty BMP image.
             * 
             * @param res new and final resolution of image.
             * @param color color type of the new image.
             */
            virtual void CreateImage(Point res, const std::unique_ptr<Color> &color) override;

            /**
             * @brief Load a BMP image from file.
             * 
             */
            virtual void LoadImage() override;

            /**
             * @brief Save the data into a BMP image.
             * 
             */
            virtual void SaveImage() override;

            /**
             * @brief Save the data into a BMP image.
             * 
             * Works like SaveImage(), but has overload for output file.
             * 
             * @param path to the file where to save the image.
             */
            virtual void SaveImage(const std::filesystem::path &path) override
            {
                file_out_ = File{FileType::kBMP, path};
                SaveImage();
            }

            /**
             * @brief Saves all images from history.
             * 
             */
            void SaveBuffer();

        private:
            HeaderBMP header_bmp_;          /// BMP header struct.
            HeaderBMPInfo header_bmp_info_; /// BMP info header struct.
            friend class ImageIOBMP;

            /**
             * @brief Checks if \ref header_bmp is valid.
             * 
             * @return true if \ref header_bmp is valid.
             * @return false if \ref header_bmp is invalid.
             */
            bool CheckHeader();

            /**
             * @brief Checks if \ref header_bmp_info_ is valid.
             * 
             * @return true if \ref header_bmp_info_ is valid.
             * @return false if \ref header_bmp_info_ is invalid.
             */
            bool CheckHeaderInfo();

            /**
             * @brief Create a data tructure for image pixels.
             * 
             */
            virtual void CreateDataBuffer() override;

            /**
             * @brief Generates new headers.
             * 
             * Takes the info in data buffer Image::image_data_ 
             * and updates the headers \ref header_bmp & \ref header_bmp_info_
             * 
             */
            virtual void GenerateMetadata() override;
        };
    }
}

#endif // PAINT_INC_IMAGE_BPM_H_
