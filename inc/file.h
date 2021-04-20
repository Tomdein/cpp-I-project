#ifndef PAINT_INC_FILE_H_
#define PAINT_INC_FILE_H_

#include <filesystem>

namespace paint
{
    /**
     * @brief Enum describing types of an image.
     * 
     */
    enum class FileType
    {
        kTIFF, /// A TIFF image.
        kPNG,  /// A PNG image.
        kBMP,  /// A BMP image.
    };

    /**
     * @brief A structure describing image file.
     * 
     */
    struct File
    {
        FileType file_type_;              /// A file path to the file.
        std::filesystem::path file_path_; /// Type of the file.
    };
}

#endif // PAINT_INC_FILE_H_