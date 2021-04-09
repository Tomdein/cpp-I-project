#ifndef PAINT_INC_FILE_H_
#define PAINT_INC_FILE_H_

#include <filesystem>

namespace paint
{

    enum class FileType
    {
        kTIFF,
        kPNG,
        kBMP,
    };

    class File
    {
    public:
    private:
        FileType file_type_;
        std::filesystem::path file_path_;
    };
}

#endif // PAINT_INC_FILE_H_