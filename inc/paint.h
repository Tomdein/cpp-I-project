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
    /**
     * @brief Paint application interface.
     * 
     * This is an interface for paint application.
     * 
     */
    class Paint
    {
    public:
        Paint() = default;
        virtual ~Paint() {}

        /**
         * @brief Starts the paint app.
         * 
         */
        virtual void Run() = 0;

    protected:
        Parser parser_;                                  /// A parser for parsing commands.
        std::vector<std::shared_ptr<Command>> commands_; /// A list of commands.
    };

    /**
     * @brief Command line interface application.
     * 
     * This is a command line interface (CLI) paint application.
     * 
     * PaintCLI::Run() starts a CLI where user can input command line by line.
     * 
     */
    class PaintCLI : public Paint
    {
    public:
        PaintCLI() {}
        virtual ~PaintCLI() override {}

        /**
         * @brief Prints help information into terminal.
         * 
         */
        static void PrintHelp();

        /**
         * @brief Starts the CLI paint app.
         * 
         */
        virtual void Run() override;
    };

    /**
     * @brief Batch converter paint app.
     * 
     * Takes commands from the given file:
     * Takes all the images from folder with given format edits them with given commands and then exports the images to selected format.
     * 
     */
    class PaintBatch : public Paint
    {
    public:
        PaintBatch(std::filesystem::path &input_file_path) : commands_file_path_{input_file_path} {}
        virtual ~PaintBatch() override {}

        /**
         * @brief Starts the batch paint app.
         * 
         */
        virtual void Run() override;

    private:
        std::filesystem::path commands_file_path_;
    };

    /**
     * @brief File paint app.
     * 
     * Creates a new image, applies the given commands from file and then saves the image.
     * 
     */
    class PaintFile : public Paint
    {
    public:
        PaintFile(std::filesystem::path input_file_path, Point resolution, std::filesystem::path output_file_path) : commands_file_path_{input_file_path},
                                                                                                                     output_resolution_{resolution},
                                                                                                                     output_file_path_{output_file_path}
        {
        }
        virtual ~PaintFile() override {}

        /**
         * @brief Starts the file paint app.
         * 
         */
        virtual void Run() override;

    private:
        std::filesystem::path commands_file_path_; /// Filepath of a file with commands.
        paint::Point output_resolution_;           /// Output resolution of the image.
        std::filesystem::path output_file_path_;   /// Filepath of the output image.
    };
}

#endif // PAINT_INT_PAINT_H_