#ifndef PAINT_INT_PAINT_H
#define PAINT_INT_PAINT_H

#include <filesystem>

#include "parser.h"
#include "point.h"

namespace paint
{
    class Paint
    {
    public:
        Paint() = default;

        virtual void Run() = 0;

    protected:
        Parser parser_;
    };

    class PaintCLI : Paint
    {
    public:
        PaintCLI();

        virtual void Run() override;
    };

    class PaintBatch : Paint
    {
    public:
        PaintBatch(std::filesystem::path input_file_path);

        virtual void Run() override;
    };

    class PaintFile : Paint
    {
    public:
        PaintFile(std::filesystem::path input_file_path, Point resolution, std::filesystem::path output_file_path);

        virtual void Run() override;
    };
}

#endif // PAINT_INT_PAINT_H