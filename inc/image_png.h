#ifndef PAINT_INC_IMAGE_PNG_H_
#define PAINT_INC_IMAGE_PNG_H_

#include <cstdint>

#include "image.h"

namespace paint
{
    namespace image_png
    {

        /**
         * @brief Signature of the file signifying it is a PNG picture.
         * 
         */
        const uint64_t kPNGSignature = 0x89504e470d0a1a0aULL;

        struct ChunkIHDR
        {
            uint32_t width;
            uint32_t height;
            uint8_t bit_depth;
            uint8_t color_type;
            uint8_t compresison_method;
            uint8_t filter_method;
            uint8_t interlace_method;
        };

        class ImagePNG : public Image
        {
        public:
            ImagePNG() = default;
            virtual ~ImagePNG() override {}

            virtual void CreateImage([[maybe_unused]] Point res, [[maybe_unused]] const std::unique_ptr<Color> &color) override {}
            virtual void LoadImage() override {}
            virtual void SaveImage() override {}
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
            * @brief Used to figure the image orientation.
            * 
            * Method used to figure if the image is drawn 'Top down' or 'Bottom up'.
            *  Top down - (0, 0) is at the bottom left of the image.
            *  Bottom up - (0, 0) is at the top left of the image.
            * 
            * @return true if data in image is from Top left to Bottom right.
            * @return false if data in image is from Bottom left to Top right.
            */
            virtual bool IsTopDown() const { return true; }

        private:
            ChunkIHDR chunk_ihdr_;

            virtual void CreateDataBuffer() override{};
            virtual void GenerateMetadata() override {}
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_H_