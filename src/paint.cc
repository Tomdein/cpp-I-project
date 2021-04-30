#include <iostream>
#include <utility>
#include <string>
#include <fstream>

#include "paint.h"

namespace paint
{
    //---------------PaintCLI--------------

    void PaintCLI::PrintHelp()
    {
        std::cout << R"!!!(

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

To To view help write -h or --help

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

        // TODO: Run each command.
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

        // TODO: Run each command.
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

        // TODO: Run each command.
    }
}