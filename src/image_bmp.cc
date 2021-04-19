#include "image_bmp.h"
#include "colors.h"

#include <fstream>

namespace paint
{
    namespace image_bmp
    {
        void ImageBMP::CreateImage()
        {
        }

        void ImageBMP::LoadImage()
        {
            try
            {
                std::ifstream file;

                // Set file to throw exceptions
                file.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

                // Open file
                file.open(file_.file_path_, std::ios::binary);

                // Read BMP headers
                file.read(reinterpret_cast<char *>(&header_bmp_), sizeof(HeaderBMP));
                file.read(reinterpret_cast<char *>(&header_bmp_info_), sizeof(HeaderBMPInfo));

                // Check if BMP headers are valid
                if (!CheckHeader() | !CheckHeaderInfo())
                {
                    throw "Invalid file format";
                }

                // Move to the start of the pixel data
                file.seekg(header_bmp_.bf_offBits);

                //TODO: Create DataPixels
                // Calculate the size of pixel data to be loaded
                size_t data_size;
                if (header_bmp_info_.bi_sizeImage == 0)
                {
                    // Calculate the size in bytes (multiply by size of pixel in bits and divide by 8)
                    data_size = std::ceil(header_bmp_info_.bi_width * header_bmp_info_.bi_height * header_bmp_info_.bi_bitCount / 8.0f);
                }
                else
                {
                    data_size = header_bmp_info_.bi_sizeImage;
                }

                // Create pixel data buffer and read pixel data
                unprocessed_data = std::make_unique<uint8_t[]>(data_size);
                file.read(reinterpret_cast<char *>(unprocessed_data.get()), data_size);

                // Create data structure for editing the pixels
                CreateDataBuffer();

                if (header_bmp_info_.bi_bitCount != k24bpPX)
                    throw "Only RGB888 is implemented.";

                std::copy_n(reinterpret_cast<uint8_t *>(unprocessed_data.get()), data_size, reinterpret_cast<uint8_t *>(*image_data_->begin()));

                file.close();

                // The data has changed (new data loaded) -> copy the data to undo history
                ImageEditCallback();

                painter.AttachImageData(image_data_);
            }
            catch (const std::ios_base::failure &fail)
            {
            }
        }

        void ImageBMP::SaveImage()
        {
            GenerateMetadata();

            try
            {
                std::ofstream file;

                // Set file to throw exceptions
                file.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

                // Open file
                //file.open("../../images/rotated.png", std::ios::binary);
                file.open(file_.file_path_, std::ios::binary);

                // Read BMP headers
                file.write(reinterpret_cast<char *>(&header_bmp_), sizeof(HeaderBMP));
                file.write(reinterpret_cast<char *>(&header_bmp_info_), sizeof(HeaderBMPInfo));

                //TODO: Create DataPixels
                // Calculate the size of pixel data to be loaded
                size_t data_size;
                if (header_bmp_info_.bi_sizeImage == 0)
                {
                    // Calculate the size in bytes (multiply by size of pixel in bits and divide by 8)
                    data_size = std::ceil(header_bmp_info_.bi_width * header_bmp_info_.bi_height * header_bmp_info_.bi_bitCount / 8.0f);
                }
                else
                {
                    data_size = header_bmp_info_.bi_sizeImage;
                }

                // How many bits after 4byte alignment
                int four_byte_align = header_bmp_info_.bi_bitCount * header_bmp_info_.bi_width % 32;
                // How many remainning bits to 4byte alignment
                if (four_byte_align != 0)
                {
                    four_byte_align = 32 - four_byte_align;

                    // How many remainning bytes to 4byte alignment, discarding the first incomplete byte, which will be copied when copying by line
                    four_byte_align = four_byte_align / 8;
                }

                int padding = 0;
                for (size_t y = 0; y < header_bmp_info_.bi_height; y++)
                {
                    file.write(reinterpret_cast<char *>((*image_data_)[y * header_bmp_info_.bi_width]),
                               std::ceil(header_bmp_info_.bi_width * static_cast<float>(header_bmp_info_.bi_bitCount) / 8));

                    if (four_byte_align)
                    {
                        file.write(reinterpret_cast<char *>(&padding), four_byte_align);
                    }
                }

                // Write pixel data
                file.write(reinterpret_cast<char *>(*image_data_->begin()), data_size);

                file.close();
            }
            catch (const std::ios_base::failure &fail)
            {
            }
        }

        bool ImageBMP::CheckHeader()
        {
            if (header_bmp_.bf_type != kBfType)
            {
                return false;
            }

            if (header_bmp_.bf_reserved1 != 0x0)
            {
                return false;
            }

            if (header_bmp_.bf_reserved2 != 0x0)
            {
                return false;
            }

            if (header_bmp_.bf_offBits < 0x36)
            {
                return false;
            }

            return true;
        }

        bool ImageBMP::CheckHeaderInfo()
        {
            if (header_bmp_info_.bi_planes != kPlanes)
            {
                return false;
            }

            if (header_bmp_info_.bi_bitCount != BiBitCount::k1bpPX &&
                header_bmp_info_.bi_bitCount != BiBitCount::k4bpPX &&
                header_bmp_info_.bi_bitCount != BiBitCount::k8bpPX &&
                header_bmp_info_.bi_bitCount != BiBitCount::k24bpPX)
            {
                return false;
            }

            if (header_bmp_info_.bi_compression != BiCompression::kBiRGB &&
                header_bmp_info_.bi_compression != BiCompression::kBiRLE8 &&
                header_bmp_info_.bi_compression != BiCompression::kBiRLE4)
            {
                return false;
            }

            return true;
        }

        void ImageBMP::CreateDataBuffer()
        {
            if (header_bmp_info_.bi_width == 0 || header_bmp_info_.bi_height == 0)
                throw "Unable to create image data. Image dimensions missing.";

            Point image_size{static_cast<int>(header_bmp_info_.bi_width), static_cast<int>(header_bmp_info_.bi_height)};

            std::unique_ptr<Color> color;
            // TODO: Create more colors for BMP
            switch (header_bmp_info_.bi_bitCount)
            {
            case k1bpPX:
                throw "Black and white color not implemented yet.";
                break;
            case k4bpPX:
                throw "4bpPX color not implemented yet.";
                break;
            case k8bpPX:
                throw "8bpPX color not implemented yet.";
                break;
            case k24bpPX:
                color = std::make_unique<ColorRGB888>(0, 0, 0);
                break;
            }

            image_data_ = std::make_shared<DataPixels>(image_size, std::move(color));
        }

        void ImageBMP::GenerateMetadata()
        {
            header_bmp_.bf_size = image_data_->GetSize().x * image_data_->GetSize().y * image_data_->GetColorType()->GetDataSize() + sizeof(HeaderBMP) + sizeof(HeaderBMPInfo);
            header_bmp_.bf_offBits = 0x36;

            header_bmp_info_.bi_size = 0x28;
            header_bmp_info_.bi_width = image_data_->GetSize().x;
            header_bmp_info_.bi_height = image_data_->GetSize().y;
            header_bmp_info_.bi_bitCount = image_data_->GetColorType()->GetDataSize() * 8;
            header_bmp_info_.bi_compression = kBiRGB;
            header_bmp_info_.bi_sizeImage = image_data_->GetSize().x * image_data_->GetSize().y * image_data_->GetColorType()->GetDataSize(); // Uncompressed size
            header_bmp_info_.bi_clrUsed = 0;
            header_bmp_info_.bi_clrImportant = 0;
        }

        void ImageBMP::SaveBuffer()
        {
            int idx = image_data_undo_history_.size() - 1;
            std::filesystem::create_directory("./image_dump");

            for (auto &img : this->image_data_undo_history_)
            {
                image_data_ = (img);

                std::stringstream s;
                s.str(std::string());
                s << "./image_dump/img_" << idx << ".bmp";
                file_ = File{FileType::kBMP, s.str()};

                SaveImage();

                idx++;
            }
        }
    }

}
