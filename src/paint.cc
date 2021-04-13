#include <iostream>
#include <utility>
#include <string>

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
            std::getline(std::cin, line);

            if ((line == "-h") || (line == "--help"))
            {
                PaintCLI::PrintHelp();
            }

            try
            {
                commands_.emplace_back(std::move(parser_.ParseLine(line)));
            }
            catch (parse_error &e)
            {
                std::cout << e.what() << ": '" << e.error_substring() << "'\n";
            }

            // Show current line in terminal
            std::cout << "> ";
        }
    }

    //--------------PaintBatch--------------
    void PaintBatch::Run()
    {
        if ((commands_file_path_.extension().string() != ".txt") | !std::filesystem::exists(commands_file_path_.parent_path()))
        {
            throw "Non-existent or unsupported file for batch convert";
        }
    }

    //---------------PaintFile--------------
    void PaintFile::Run()
    {
    }
}