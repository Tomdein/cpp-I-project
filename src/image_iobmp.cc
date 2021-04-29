#include "image_iobmp.h"

#include "color_grayscale.h"
#include "color_bw.h"

namespace paint
{
    namespace image_bmp
    {
        void ImageIOBMP::ReadHeaderBMP(std::ifstream &file, ImageBMP &image)
        {
            // Read the whole header
            file.read(reinterpret_cast<char *>(&image.header_bmp_), sizeof(HeaderBMP));
        }

        void ImageIOBMP::ReadHeaderBMPInfo(std::ifstream &file, ImageBMP &image)
        {
            // Read the whole header
            file.read(reinterpret_cast<char *>(&image.header_bmp_info_), sizeof(HeaderBMPInfo));
        }

        void ImageIOBMP::ReadPixelData(std::ifstream &file, ImageBMP &image)
        {
            // Calculate the size of pixel data to be loaded
            size_t data_size = image.header_bmp_info_.bi_sizeImage;
            if (data_size == 0)
            {
                // Size in DWORD (4 bytes)
                data_size = (image.header_bmp_info_.bi_width * image.header_bmp_info_.bi_bitCount + 31) / 32 * image.header_bmp_info_.bi_height;
                // Uncompressed size in single bytes
                data_size = data_size * 4;
            }

            // Create pixel data buffer and read pixel data
            image.unprocessed_data = std::make_unique<uint8_t[]>(data_size);
            file.read(reinterpret_cast<char *>(image.unprocessed_data.get()), data_size);

            // How many bits after 4byte alignment
            int four_byte_align = (image.header_bmp_info_.bi_bitCount * image.header_bmp_info_.bi_width) % 32;
            // How many remainning bits to 4byte alignment
            if (four_byte_align != 0)
            {
                four_byte_align = 32 - four_byte_align;

                // How many remainning bytes to 4byte alignment, discarding the first incomplete byte, which will be copied when copying by line
                four_byte_align = four_byte_align / 8;
            }

            // Number of bytes in pixel row without padding
            const int data_width_bytes = std::ceil(static_cast<float>(image.header_bmp_info_.bi_width * image.header_bmp_info_.bi_bitCount) / 8);

            // If pixel is multiple of byte
            if (image.header_bmp_info_.bi_bitCount == BiBitCount::k24bpPX ||
                image.header_bmp_info_.bi_bitCount == BiBitCount::k16bpPX ||
                image.header_bmp_info_.bi_bitCount == BiBitCount::k8bpPX)
            {
                // For each row
                for (size_t y = 0; y < image.header_bmp_info_.bi_height; y++)
                {
                    // Copy whole row
                    std::copy_n(reinterpret_cast<uint8_t *>(image.unprocessed_data.get()) + y * data_width_bytes, data_width_bytes, reinterpret_cast<uint8_t *>(image.image_data_->at(0, y)));
                }
            }
            // Each pixel size is not multiple of byte
            else
            {

                const uint8_t pixel_mask = std::pow(2, image.header_bmp_info_.bi_bitCount) - 1; // Mask for the pixel
                uint8_t mask;                                                                   // Mask shifted to the right position
                int pos;                                                                        // Position of the byte
                uint8_t pixel;                                                                  // Value of the pixel after applying mask

                // For each row
                for (size_t y = 0; y < image.header_bmp_info_.bi_height; y++)
                {
                    // For each pixel in row
                    for (size_t x = 0; x < image.header_bmp_info_.bi_width; x++)
                    {
                        // Calculate byte position
                        pos = x / image.header_bmp_info_.bi_bitCount;
                        // Shift the mask
                        mask = pixel_mask << (x % image.header_bmp_info_.bi_bitCount);
                        // Get the value
                        pixel = image.unprocessed_data.get()[y * data_width_bytes + pos] & mask;

                        // Set the value in data at (x, y)
                        *reinterpret_cast<uint8_t *>(image.image_data_->at(x, y)) = pixel;
                    }
                }
            }

            // Free copied data
            delete[] image.unprocessed_data.release();
        }

        void ImageIOBMP::WriteHeaderBMP(std::ofstream &file, ImageBMP &image)
        {
            // Write the whole header
            file.write(reinterpret_cast<char *>(&image.header_bmp_), sizeof(HeaderBMP));
        }

        void ImageIOBMP::WriteHeaderBMPInfo(std::ofstream &file, ImageBMP &image)
        {
            // Write the whole header
            file.write(reinterpret_cast<char *>(&image.header_bmp_info_), sizeof(HeaderBMPInfo));
        }

        void ImageIOBMP::WriteColorTable(std::ofstream &file, ImageBMP &image)
        {
            switch (image.header_bmp_info_.bi_bitCount)
            {
            case BiBitCount::k1bpPX:
                file.write(reinterpret_cast<const char *>(paint::bw_palette), sizeof(bw_palette));
                break;

            case BiBitCount::k8bpPX:
                file.write(reinterpret_cast<const char *>(paint::grayscale_palette), sizeof(grayscale_palette));
                break;
            }
        }

        void ImageIOBMP::WritePixelData(std::ofstream &file, ImageBMP &image)
        {
            // Write the color table
            WriteColorTable(file, image);

            // How many bits after 4byte alignment
            int four_byte_align = (image.header_bmp_info_.bi_bitCount * image.header_bmp_info_.bi_width) % 32;
            // How many remainning bits to 4byte alignment
            if (four_byte_align != 0)
            {
                four_byte_align = 32 - four_byte_align;

                // How many remainning bytes to 4byte alignment, discarding the first incomplete byte, which will be copied when copying by line
                four_byte_align = four_byte_align / 8;
            }

            // Number of bytes in pixel row without padding
            const int data_width_bytes = std::ceil(static_cast<float>(image.header_bmp_info_.bi_width * image.header_bmp_info_.bi_bitCount) / 8);

            // Just bunch of 0's used for padding the row
            const int padding = 0;

            // If pixel is multiple of byte
            if (image.header_bmp_info_.bi_bitCount == BiBitCount::k24bpPX ||
                image.header_bmp_info_.bi_bitCount == BiBitCount::k16bpPX ||
                image.header_bmp_info_.bi_bitCount == BiBitCount::k8bpPX)
            {
                for (size_t y = 0; y < image.header_bmp_info_.bi_height; y++)
                {
                    file.write(reinterpret_cast<char *>((*image.image_data_)[y * image.header_bmp_info_.bi_width]), data_width_bytes);

                    if (four_byte_align)
                    {
                        file.write(reinterpret_cast<const char *>(&padding), four_byte_align);
                    }
                }
            }
            else
            {
                size_t colors_per_byte = 8 / image.header_bmp_info_.bi_bitCount;

                char data_byte = 0;
                // For each line
                for (size_t y = 0; y < image.header_bmp_info_.bi_height; y++)
                {
                    // For each byte in line
                    for (int x = 0; x < data_width_bytes; x++)
                    {
                        // Clear compressed data_byte
                        data_byte = 0;

                        for (size_t i = 0; i < colors_per_byte; i++)
                        {
                            // Set bits in data_byte
                            int idx = y * image.header_bmp_info_.bi_width + x * 8 + i;
                            int shift = (colors_per_byte - i - 1) * image.header_bmp_info_.bi_bitCount;
                            unsigned char val;
                            if (image.header_bmp_info_.bi_bitCount == BiBitCount::k1bpPX)
                            {
                                val = (*reinterpret_cast<const PixelBW *>((*image.image_data_)[idx])).w;
                            }
                            else
                            {
                                throw "todo BiBitCount::k4bpPX"; // not done
                            }

                            data_byte |= val << shift;

                            // data_byte |= (*reinterpret_cast<const char *>((*image.image_data_)[y * image.header_bmp_info_.bi_width + x * 8 + i])) << (i * image.header_bmp_info_.bi_bitCount);
                        }

                        // Write the single compressed byte;
                        file.write(&data_byte, 1);
                    }

                    // Add padding at the end of line
                    if (four_byte_align)
                    {
                        file.write(reinterpret_cast<const char *>(&padding), four_byte_align);
                    }
                }

                // throw "TODO BMP BiBitCount::k1bpPX && BiBitCount::k4bpPX support";
            }
        }
    }
}