#include <filesystem>
#include <iostream>
#include <string>
#include <regex>

#include "paint.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        paint::PaintCLI app;
        app.Run();
    }
    if (argc == 2)
    {
        std::filesystem::path path(argv[1]);
        paint::PaintBatch app(path);
        app.Run();
    }
}
