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
        if (!std::filesystem::exists(commands_file_path_))
        {
            std::cerr << "File with command does not exist: " << commands_file_path_.string() << std::endl;
            throw "File with command does not exist";
        }

        if (commands_file_path_.extension() != ".txt")
        {
            std::cerr << "Unsupported extension '" << commands_file_path_.extension().string() << "' in batch convert file: " << commands_file_path_.string() << std::endl;
            throw "Unsupported file for batch convert";
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
        {
            std::cerr << "PaintCLI::Run - No commands to apply." << std::endl;
            return;
        }

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

        // Check file type
        auto file_path = load_command->FilePath();
        auto file_ext = file_path.extension();
        if (file_ext.string() != ".png" && file_ext.string() != ".bmp")
        {
            std::cerr << "Unknown file type: '" << file_ext.string() << "', " << file_path << std::endl;
            throw unknown_file_error(file_path);
        }

        for (auto &dir_entry : std::filesystem::directory_iterator(file_path))
        {
            // Try reading all files
            try
            {
                if (dir_entry.path().extension() == file_ext && dir_entry.is_regular_file())
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

        if (commands_.empty())
        {
            std::cerr << "PaintFile::Run - No commands to apply." << std::endl;
            return;
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

        // Prepare file
        std::ifstream file(commands_file_path_);
        if (!file.is_open())
            perror(("error while opening file: " + commands_file_path_.string()).c_str());

        // Open and parse the file
        bool failed_to_parse_command = false;
        auto command_counter = 0;

        bool exit = false;
        std::string line;
        std::shared_ptr<Command> command;

        // Read commands file
        while (!exit)
        {
            // Read a line
            if (!std::getline(file, line))
                break;

            command_counter++;

            // Try parsing the input as command
            try
            {
                commands_.emplace_back(std::move(parser_.ParseLine(line)));
            }
            catch (parse_error &e)
            {
                std::cerr << e.what() << " [" << command_counter << "]: " << e.error_substring() << "\n";
                failed_to_parse_command = true;
            }
        }

        // Ask whether to continue if Painter failed to parse any command
        if (failed_to_parse_command)
        {
            std::cout << "Painter failed to parse some commands. Continue to save? [Y/N]" << std::endl;
            std::string confirmation;
            std::cin >> confirmation;

            if (confirmation != "Y" && confirmation != "y")
                return;
        }

        // No commands entered -> end app
        if (commands_.empty())
        {
            std::cerr << "PaintFile::Run - No commands to apply." << std::endl;
            return;
        }

        // Apply all valid parsed commands
        std::for_each(commands_.begin(), commands_.end(), [this](auto &command) {
            command->Invoke(*image_);
        });

        // Resize the image to final size and save
        image_->painter.Resize(PointPX(output_resolution_.x, output_resolution_.y));
        image_->DumpImageHistory();
        image_->SaveImage(output_file_path_);
    }
}