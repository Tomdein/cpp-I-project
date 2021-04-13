#ifndef PAINT_INT_PAINT_H_
#define PAINT_INT_PAINT_H_

#include <vector>
#include <memory>
#include <filesystem>

#include "point.h"
#include "command.h"
#include "parser.h"

namespace paint
{
    class Paint
    {
    public:
        Paint() = default;

        virtual void Run() = 0;

    protected:
        Parser parser_;
        std::vector<std::shared_ptr<Command>> commands_;
    };

    class PaintCLI : public Paint
    {
    public:
        PaintCLI(){};

        static void PrintHelp();
        virtual void Run() override;
    };

    class PaintBatch : public Paint
    {
    public:
        PaintBatch(std::filesystem::path &input_file_path) : commands_file_path_{input_file_path} {}

        virtual void Run() override;

    private:
        std::filesystem::path commands_file_path_;
    };

    class PaintFile : public Paint
    {
    public:
        PaintFile(std::filesystem::path input_file_path, Point resolution, std::filesystem::path output_file_path) : commands_file_path_{input_file_path},
                                                                                                                     output_resolution_{resolution},
                                                                                                                     output_file_path_{output_file_path}
        {
        }

        virtual void Run() override;

    private:
        std::filesystem::path commands_file_path_;
        paint::Point output_resolution_;
        std::filesystem::path output_file_path_;
    };
}

#endif // PAINT_INT_PAINT_H_