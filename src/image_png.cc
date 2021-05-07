#include "image_png.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <functional>

#include "zlib.h"

#include "image_iopng.h"

namespace paint
{
    namespace image_png
    {

        void ImagePNG::LoadImage()
        {
            std::ifstream file;

            // Set file to throw exceptions
            file.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);

            try
            {
                // Open input file
                file.open(file_in_.file_path_, std::ios::binary);

                // Load all chunks from PNG file
                ImageIOPNG::ReadPNGFile(file, *this);
            }
            catch (const std::ios_base::failure &fail)
            {
                std::cerr << "Error loading PNG image from file: " << file_in_.file_path_.string() << std::endl;
                throw fail;
            }

            // Load data from IDAT chunks to DataPixels
            ImageIOPNG::ReadIDATChunks(*this);
        }

        void ImagePNG::SaveImage()
        {
            // Create all chunks etc.
            GenerateMetadata();

            std::ofstream file;

            // Set file to throw exceptions
            file.exceptions(std::ios_base::eofbit | std::ios_base::badbit | std::ios_base::failbit);

            try
            {
                // Open output file
                file.open(file_out_.file_path_, std::ios_base::binary);

                // Write PNG signature
                file.write(reinterpret_cast<const char *>(&kPNGSignature), sizeof(kPNGSignature));

                std::shared_ptr<ChunkPNG> chunk;

                // Write IHDR chunk
                chunk = chunk_ihdr_;
                ImageIOPNG::WriteChunkPNG(file, chunk);

                // Write all chunks that shall be before PLTE and IDAT chunk
                std::for_each(vec_chunk_pre_plte_.begin(), vec_chunk_pre_plte_.end(), [&file](auto chunk_it) { if(chunk_it->SafeToCopy()) ImageIOPNG::WriteChunkPNG(file, chunk_it); });

                // Write PLTE chunk (if PLTE chunk exists)
                chunk = chunk_plte_;
                if (chunk)
                    ImageIOPNG::WriteChunkPNG(file, chunk);

                // Write all chunks that shall be before IDAT chunk
                std::for_each(vec_chunk_pre_idat_.begin(), vec_chunk_pre_idat_.end(), [&file](auto chunk_it) { if(chunk_it->SafeToCopy()) ImageIOPNG::WriteChunkPNG(file, chunk_it); });

                // Write all IDAT chunks
                std::for_each(vec_chunk_data_.begin(), vec_chunk_data_.end(), [&file](auto chunk_it) { if(chunk_it->SafeToCopy()) ImageIOPNG::WriteChunkPNG(file, chunk_it); });

                // Write all chunks that can be anywhere
                std::for_each(vec_chunk_pre_none.begin(), vec_chunk_pre_none.end(), [&file](auto chunk_it) { if(chunk_it->SafeToCopy()) ImageIOPNG::WriteChunkPNG(file, chunk_it); });

                // Write IEND chunk
                chunk = chunk_iend_;
                ImageIOPNG::WriteChunkPNG(file, chunk);
            }
            catch (const std::ios_base::failure &fail)
            {
                std::cerr << "Error writing PNG image to file: " << file_out_.file_path_.string() << std::endl;
                throw fail;
            }
        }

        void ImagePNG::GenerateMetadata()
        {
            ImageIOPNG::GenerateIHDRChunks(*this);

            //TODO: PLTE chunk support
            chunk_plte_.reset();

            ImageIOPNG::GenerateIDATChunks(*this);
        }
    }
}