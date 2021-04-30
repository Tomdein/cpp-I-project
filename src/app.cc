#include <filesystem>
#include <iostream>
#include <string>
#include <regex>

#include "paint.h"
#include "parser.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        // Create app
        paint::PaintCLI app;

        // Run app
        app.Run();
    }
    else if (argc == 2)
    {
        // Get input file name from args
        std::filesystem::path path_in(argv[1]);

        // Create app
        paint::PaintBatch app(path_in);

        // Run app
        app.Run();
    }
    else if (argc == 4)
    {
        // Get input and output file name from args
        std::filesystem::path path_in(argv[1]);
        std::filesystem::path path_out(argv[3]);

        // Get resolution from args
        std::string string_res(argv[2]);
        std::smatch match;
        std::regex re_resolution("^(\\d*),(\\d*)$");

        // Match the resolution with regex
        if (!std::regex_match(string_res, match, re_resolution))
            throw paint::parse_error(argv[2]);

        // Create app
        paint::PaintFile app(path_in, paint::Point{stoi(match[1].str()), stoi(match[2].str())}, path_out);

        // Run app
        app.Run();
    }
}
