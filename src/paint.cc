#include <iostream>
#include <utility>
#include <string>
#include <fstream>
#include <algorithm>

#include "paint.h"
#include "color_rgb888.h"

namespace paint
{

    const char *unknown_file_error::what() const noexcept
    {
        return "Unknown file";
    }

    const char *unknown_file_error::error_file() const noexcept
    {
        return error_filename_;
    }

    unknown_file_error::unknown_file_error(const char *error_filename)
    {
        std::strncpy(error_filename_, error_filename, kFilenameErrorLength);
        error_filename_[kFilenameErrorLength - 1] = '\0';
    }

    unknown_file_error::unknown_file_error(const std::string &error_filename)
    {
        size_t n = error_filename.copy(error_filename_, kFilenameErrorLength);
        if (n != kFilenameErrorLength)
        {
            error_filename_[n] = '\0';
        }
        else
        {
            error_filename_[kFilenameErrorLength] = '\0';
        }
    }

    //---------------PaintCLI--------------

    void PaintCLI::PrintHelp()
    {
        std::cout << R"!!!(
    Some commands can have optional arguments.
    The optional arguments are in curly braces '{}'.
    The optional arguments can have random order.
    If no optional arguments are entered, leave out the curly braces.

    Supported commands are:

        LOAD ./folder/filename.bmp or LOAD ./folder/*.bmp
        SAVE ./folder/filename.bmp or SAVE ./folder/*.bmp
        
        COLOR r g b
        
        LINE %|PX x1 y1 x2 y2 {
                               width: number,
                               color: {r: number, g: number, b: number}
                               }
        
        CIRCLE %|PX x1 y1 radius {
                                  fill: bool,
                                  fill-color: {r: number, g: number, b: number},
                                  border-color: {r: number, g: number, b: number},
                                  border-width: number
                                  }
        
        BUCKET %|PX x1 y1 {
                           color: {r: number, g: number, b: number
                           }
        
        RESIZE %|PX width height
        
        ROTATE CLOCK|COUNTERCLOCK

        INVERTCOLORS
        GRAYSCALE

        CROP %|PX x1 y1 x2 y2

        UNDO
        REDO


)!!!";
    } // PrintHelp end

    void Paint::CheckCommandFile(std::filesystem::path &commands_file_path_)
    {
        if ((commands_file_path_.extension().string() != ".txt") | !std::filesystem::exists(commands_file_path_.parent_path()))
        {
            throw "Non-existent or unsupported file for batch convert";
        }

        if (!std::filesystem::exists(commands_file_path_))
        {
            throw "File with command does not exist";
        }
    }

    void PaintCLI::Run()
    {
        bool exit = false;
        std::string line;
        std::shared_ptr<Command> command;

        std::cout << R"!!!(

Welcome to PaintCLI

To view help write -h or --help, to quit write q

)!!!";

        // Show current line in terminal
        std::cout
            << "> ";

        while (!exit)
        {
            // Read a line
            std::getline(std::cin, line);

            // If input is '-h' or '--help' -> show help and continue;
            if ((line == "-h") || (line == "--help"))
            {
                PaintCLI::PrintHelp();

                // Show current line in terminal
                std::cout << "> ";

                continue;
            }

            // Quit CLI
            if (line == "q")
                break;

            // Try parsing the input as command
            try
            {
                commands_.emplace_back(std::move(parser_.ParseLine(line)));
            }
            catch (parse_error &e)
            {
                std::cerr << e.what() << ": '" << e.error_substring() << "'\n";
            }

            // Show current line in terminal
            std::cout << "> ";
        }

        // No commands entered -> end app
        if (commands_.empty())
            return;

        auto load_command = std::dynamic_pointer_cast<LoadCommand>(commands_.front());
        if (!load_command)
        {
            std::cerr << "First command is not LOAD command!" << std::endl;
            throw "First command is not LOAD command!";
        }

        image_ = CreateImageByExtension(load_command->FilePath());
        image_->LoadImage();

        std::for_each(++commands_.begin(), commands_.end(), [this](auto &command) {
            command->Invoke(*image_);
        });
    }

    //--------------PaintBatch--------------
    void PaintBatch::Run()
    {
        // Check the commands file
        try
        {
            CheckCommandFile(commands_file_path_);
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }

        std::ifstream file;

        file.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);

        bool exit = false;
        std::string line;
        std::shared_ptr<Command> command;

        while (!exit)
        {
            // Read a line
            std::getline(file, line);

            // Try parsing the input as command
            try
            {
                commands_.emplace_back(std::move(parser_.ParseLine(line)));
            }
            catch (parse_error &e)
            {
                std::cerr << e.what() << ": '" << e.error_substring() << "'\n";
            }
        }

        // No commands entered -> end app
        if (commands_.empty())
            return;

        auto load_command = std::dynamic_pointer_cast<LoadCommand>(commands_.front());
        if (!load_command)
        {
            std::cerr << "First command is not LOAD command!" << std::endl;
            throw "First command is not LOAD command!";
        }

        for (auto &dir_entry : std::filesystem::directory_iterator(load_command->FilePath()))
        {
            try
            {
                if (dir_entry.is_regular_file())
                {
                    auto img = CreateImageByExtension(dir_entry.path());
                    img->LoadImage();
                    images_.emplace_back(std::move(img));
                }
            }
            // Unknown file -> do nothing
            catch (const unknown_file_error &e)
            {
                // Do nothing
            }
        }

        // Run each command.
        std::for_each(images_.begin(), images_.end(), [this](auto &img) {
            std::for_each(++commands_.begin(), commands_.end(), [&img](auto &command) {
                command->Invoke(*img);
            });
        });
    }

    //---------------PaintFile--------------
    void PaintFile::Run()
    {
        // Check the commands file
        try
        {
            CheckCommandFile(commands_file_path_);
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }

        // Create image
        try
        {
            image_ = CreateImageByExtension(output_file_path_);
            image_->CreateImage(output_resolution_, std::make_unique<ColorRGB888>(0, 0, 0));
        }
        catch (unknown_file_error &e)
        {
            std::cerr << e.what() << e.error_file() << std::endl;
            return;
        }

        std::ifstream file;

        file.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);

        bool exit = false;
        std::string line;
        std::shared_ptr<Command> command;

        // Read commands file
        while (!exit)
        {
            // Read a line
            std::getline(file, line);

            // Try parsing the input as command
            try
            {
                commands_.emplace_back(std::move(parser_.ParseLine(line)));
            }
            catch (parse_error &e)
            {
                std::cerr << e.what() << ": '" << e.error_substring() << "'\n";
            }
        }

        // TODO: Run each command.
        std::for_each(commands_.begin(), commands_.end(), [this](auto &command) {
            if (command->CommandName() == "SaveCommand")
                image_->painter.Resize(PointPX(output_resolution_.x, output_resolution_.y));

            command->Invoke(*image_);
        });
    }
}