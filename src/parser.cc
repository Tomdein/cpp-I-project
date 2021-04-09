#include <string>
#include <memory>
#include <regex>

#include "command.h"
#include "parser.h"

namespace paint
{
    std::shared_ptr<Command> Parser::ParseLine(std::string &line)
    {
        std::smatch match;
        if (std::regex_match(line, match, Parser::re_load_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_save_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_color_))
        {
            std::make_shared<paint::Command>(paint::ColorCommand(std::move(std::make_shared<paint::Color>(new paint::ColorRGB888(0, 0, 0)))));
        }
        else if (std::regex_match(line, match, Parser::re_line_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_circle_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_bucket_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_resize_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_rotate_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_invert_colors_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_grayscale_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_crop_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_undo_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_redo_))
        {
        }

        throw paint::parse_error("Unable to parse command", line);
    }
}