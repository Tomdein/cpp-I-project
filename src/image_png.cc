#include "image_png.h"
#include "image_iopng.h"

#include <iostream>
#include <fstream>
#include <memory>

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

                chunk_ihdr_ = std::reinterpret_pointer_cast<ChunkIHDR>(chunk);

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
                        if (chunk_plte_.get())
                        {
                            std::cerr << "PLTE chunk already exists!" << std::endl;
                            throw error_chunk_mismatch();
                        }

                        chunk_plte_ = std::reinterpret_pointer_cast<ChunkPLTE>(chunk);
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
                    if (!chunk_plte_.get())
                    {
                        vec_chunk_pre_plte_.emplace_back(std::move(chunk));
                    }
                    else
                    {
                        vec_chunk_pre_idat_.emplace_back(std::move(chunk));
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

                    vec_chunk_pre_none.emplace_back(std::move(chunk));
                }

                // Set IEND chunk
                chunk_iend_ = std::reinterpret_pointer_cast<ChunkIEND>(chunk);

                // Last chunk must be IEND chunk
                // DONE READING PNG
            }
            catch (const std::ios_base::failure &fail)
            {
                std::cerr << "Error loading PNG image from file: " << file_in_.file_path_.string() << std::endl;
                throw fail;
            }
        }

        void ImagePNG::SaveImage()
        {
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
    }
}