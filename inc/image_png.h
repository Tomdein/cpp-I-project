#ifndef PAINT_INC_IMAGE_PNG_H_
#define PAINT_INC_IMAGE_PNG_H_

#include <cstdint>
#include <vector>

#include "image.h"
#include "image_png_chunk.h"

namespace paint
{
    namespace image_png
    {

        /**
        * @brief A exception class
        * 
        * \ref error_bad_chunk is thrown when given data cannot be reinterpreted as a chunk or the data is invalid
        * 
        */
        class error_bad_chunk : public std::exception
        {
        public:
            error_bad_chunk() = default;
            virtual ~error_bad_chunk() override {}

            virtual const char *what() const noexcept override { return "Given chunk data is invalid"; }
        };

        /**
        * @brief A exception class
        * 
        * \ref error_bad_chunk is thrown when given data cannot be reinterpreted as a chunk or the data is invalid
        * 
        */
        class error_chunk_mismatch : public std::exception
        {
        public:
            error_chunk_mismatch() = default;
            virtual ~error_chunk_mismatch() override {}

            virtual const char *what() const noexcept override { return "PNG chunks in given file has invalid chunk ordering"; }
        };

        class ImagePNG : public Image
        {
        public:
            ImagePNG() = default;
            virtual ~ImagePNG() override {}

            virtual void CreateImage([[maybe_unused]] Point res, [[maybe_unused]] const std::unique_ptr<Color> &color) override {}
            virtual void LoadImage() override;
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

            virtual void CreateDataBuffer() override{};
            virtual void GenerateMetadata() override;

        private:
            std::shared_ptr<ChunkIHDR> chunk_ihdr_;                  /// IHDR chunk (shall be first).
            std::shared_ptr<ChunkPLTE> chunk_plte_;                  /// PLTE chunk (before first IDAT chunk).
            std::vector<std::shared_ptr<ChunkIDAT>> vec_chunk_data_; /// Vector of IDATA chunks (shall be consecutive, each IDAT chunk contains pixel data).
            std::shared_ptr<ChunkIEND> chunk_iend_;                  /// IDEN chunk (shall be last).

            std::vector<std::shared_ptr<ChunkPNG>> vec_chunk_pre_plte_; /// Chunks that must be before PLTE and IDAT chunks.
            std::vector<std::shared_ptr<ChunkPNG>> vec_chunk_pre_idat_; /// Chunks that must be before IDAT chunk.
            std::vector<std::shared_ptr<ChunkPNG>> vec_chunk_pre_none;  /// Chunks that can be anywhere.

            friend class ImageIOPNG;

            void WriteIDATChunks();

            bool image_loaded_ = false;
        };
    }
}

#endif // PAINT_INC_IMAGE_PNG_H_