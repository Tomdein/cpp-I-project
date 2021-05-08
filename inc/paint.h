#ifndef PAINT_INT_PAINT_H_
#define PAINT_INT_PAINT_H_

#include <vector>
#include <memory>
#include <vector>
#include <filesystem>

#include "point.h"
#include "command.h"
#include "parser.h"
#include "image.h"
#include "image_png.h"
#include "image_bmp.h"

namespace paint
{
    class unknown_file_error : public std::exception
    {
    public:
        unknown_file_error(const char *error_filename);

        unknown_file_error(const std::string &error_filename);

        virtual ~unknown_file_error() override{};

        /**
         * @brief Return what caused the exception.
         * 
         * @return const char* message describing what caused the exception.
         */
        virtual const char *what() const noexcept override;

        /**
         * @brief Get the filename that caused the exception.
         * 
         * 
         * @return const char* cstring containing the filename that caused the error.
         */
        virtual const char *error_file() const noexcept;

    private:
        /**
         * @brief A constant that define the maximum length of the filename.
         * 
         */
        enum
        {
            kFilenameErrorLength = 256, /// Maximum length of the filename.
        };

        char error_filename_[kFilenameErrorLength]; /// Array storing error filename.
    };

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

        /**
         * @brief Checks whether input file with commands is valid.
         * 
         * Checks whether input file with commands is a *.txt file and that the given file exists.
         * 
         */
        void CheckCommandFile(std::filesystem::path &commands_file_path_);

        std::unique_ptr<Image> CreateImageByExtension(const std::filesystem::path &file_path)
        {
            auto extension = file_path.extension();

            if (extension == ".png")
            {
                return std::make_unique<image_png::ImagePNG>();
            }
            else if (extension == ".bmp")
            {
                return std::make_unique<image_bmp::ImageBMP>(file_path);
            }
            else
            {
                std::cerr << "Unsupported file/image type!" << std::endl;
                throw unknown_file_error(file_path.string());
            }
        }
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

    private:
        std::unique_ptr<Image> image_;
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
        std::vector<std::unique_ptr<Image>> images_;
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
        std::unique_ptr<Image> image_; /// Image to draw onto.

        std::filesystem::path commands_file_path_; /// Filepath of a file with commands.
        paint::Point output_resolution_;           /// Output resolution of the image.
        std::filesystem::path output_file_path_;   /// Filepath of the output image.
    };
}

#endif // PAINT_INT_PAINT_H_