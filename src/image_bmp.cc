//#include "image_bmp.h"
#include "image_iobmp.h"
#include "colors.h"

#include <fstream>
#include <iostream>

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

                // Open input file
                file.open(file_in_.file_path_, std::ios::binary);

                // Read BMP headers
                ImageIOBMP::ReadHeaderBMP(file, *this);
                ImageIOBMP::ReadHeaderBMPInfo(file, *this);

                // Check if BMP headers are valid
                if (!CheckHeader() | !CheckHeaderInfo())
                {
                    throw "Invalid file format";
                }

                if (header_bmp_info_.bi_bitCount != BiBitCount::k24bpPX &&
                    header_bmp_info_.bi_bitCount != BiBitCount::k16bpPX &&
                    header_bmp_info_.bi_bitCount != BiBitCount::k8bpPX &&
                    header_bmp_info_.bi_bitCount != BiBitCount::k1bpPX)
                    throw "Only RGB888 & RGB565 & Grayscale & BW is implemented.";

                // Create data structure for editing the pixels
                CreateDataBuffer();

                // Move to the start of the pixel data
                file.seekg(header_bmp_.bf_offBits);

                // Read the pixel data
                ImageIOBMP::ReadPixelData(file, *this);

                file.close();

                // Attach data to the image painter
                painter.AttachImageData(image_data_);

                // The data has changed (new data loaded) -> copy the data to undo history
                ImageEditCallback();
            }
            catch (const std::ios_base::failure &fail)
            {
                std::cout << "Error reading from file: " << file_in_.file_path_.string() << std::endl;
                throw fail;
            }
        }

        void ImageBMP::SaveImage()
        {
            // Renew the headers
            GenerateMetadata();

            try
            {
                std::ofstream file;

                // Set file to throw exceptions
                file.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

                // Open output file
                file.open(file_out_.file_path_, std::ios::binary);

                // Write BMP headers
                ImageIOBMP::WriteHeaderBMP(file, *this);
                ImageIOBMP::WriteHeaderBMPInfo(file, *this);

                // Write pixel data
                ImageIOBMP::WritePixelData(file, *this);

                file.close();
            }
            catch (const std::ios_base::failure &fail)
            {
                std::cout << "Error writing to file: " << file_out_.file_path_.string() << std::endl;
                throw fail;
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
            // Check if dimensions in BMP header are valid
            if (header_bmp_info_.bi_width == 0 || header_bmp_info_.bi_height == 0)
                throw "Unable to create image data. Image dimensions missing.";

            // Get the dimensions from BMP header
            Point image_size{static_cast<int>(header_bmp_info_.bi_width), static_cast<int>(header_bmp_info_.bi_height)};

            // Prepare color
            std::unique_ptr<Color> color;

            // TODO: Create more colors for BMP
            // Select color based on BMP header
            switch (header_bmp_info_.bi_bitCount)
            {

            // Black and white (or mapped color)
            case k1bpPX:
                color = std::make_unique<ColorBW>(0);
                break;

            case k4bpPX:
                throw "4bpPX color not implemented yet.";
                break;

            // Grayscale (or mapped color)
            case k8bpPX:
                color = std::make_unique<ColorGrayscale>(0);
                break;

            // RGB565
            case k16bpPX:
                color = std::make_unique<ColorGrayscale>(0);
                break;

            // RGB888
            case k24bpPX:
                color = std::make_unique<ColorRGB888>(0, 0, 0);
                break;
            }

            // Create new data
            image_data_ = std::make_shared<DataPixels>(image_size, std::move(color));
        }

        void ImageBMP::GenerateMetadata()
        {
            // Generate headers from image data
            header_bmp_info_.bi_size = 0x28;
            header_bmp_info_.bi_width = image_data_->GetSize().x;
            header_bmp_info_.bi_height = image_data_->GetSize().y;
            header_bmp_info_.bi_bitCount = image_data_->GetColorType()->GetDataSizeBits();
            header_bmp_info_.bi_compression = kBiRGB;
            header_bmp_info_.bi_sizeImage = (header_bmp_info_.bi_width * header_bmp_info_.bi_bitCount + 31) / 32 * header_bmp_info_.bi_height * 4; // Uncompressed size
            header_bmp_info_.bi_clrUsed = 0;
            header_bmp_info_.bi_clrImportant = 0;

            header_bmp_.bf_size = header_bmp_info_.bi_sizeImage + sizeof(HeaderBMP) + sizeof(HeaderBMPInfo);
            header_bmp_.bf_offBits = 0x36;
        }

        void ImageBMP::SaveBuffer()
        {

            throw "TODO";

            // Create image_dump dir
            std::filesystem::create_directory("./image_dump");

            int idx = 0;

            // Go through every picture in undo history and save it
            for ([[maybe_unused]] auto &img : this->image_data_undo_history_)
            {
                // Set file name and filepath
                std::stringstream s;
                s.str(std::string());
                s << "./image_dump/img_" << idx << ".bmp";
                file_out_ = File{FileType::kBMP, s.str()};

                // Save the image
                SaveImage();

                idx++;
            }
        }
    }

}
