#include <string>
#include <memory>
#include <regex>

#include "command.h"
#include "parser.h"

namespace paint
{
    const char *parse_error::what() const noexcept
    {
        return "Exception while parsing ";
    }

    const char *parse_error::error_substring() const noexcept
    {
        return error_substring_;
    }

    std::regex Parser::re_save_ = std::regex("^$", std::regex::ECMAScript);
    std::regex Parser::re_load_ = std::regex("^$", std::regex::ECMAScript);
    std::regex Parser::re_color_ = std::regex("^COLOR\\s(\\d{1,3})\\s(\\d{1,3})\\s(\\d{1,3})$", std::regex::ECMAScript);
    std::regex Parser::re_line_ = std::regex("^LINE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.*)\\})?}$", std::regex::ECMAScript);
    std::regex Parser::re_circle_ = std::regex("^CIRCLE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.*)\\})?$", std::regex::ECMAScript);
    std::regex Parser::re_bucket_ = std::regex("^BUCKET\\s(%|PX)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.*)\\})?$", std::regex::ECMAScript);
    std::regex Parser::re_resize_ = std::regex("^RESIZE\\s(%|PX)\\s(\\d+)\\s(\\d+)$", std::regex::ECMAScript);
    std::regex Parser::re_rotate_ = std::regex("^ROTATE\\s(CLOCK|COUNTERCLOCK)$", std::regex::ECMAScript);
    std::regex Parser::re_invert_colors_ = std::regex("^INVERTCOLORS$", std::regex::ECMAScript);
    std::regex Parser::re_grayscale_ = std::regex("^GRAYSCALE$", std::regex::ECMAScript);
    std::regex Parser::re_crop_ = std::regex("^CROP\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)$", std::regex::ECMAScript);
    std::regex Parser::re_undo_ = std::regex("^UNDO$", std::regex::ECMAScript);
    std::regex Parser::re_redo_ = std::regex("^REDO$", std::regex::ECMAScript);
    std::regex Parser::re_param_delim_ = std::regex(",\\s(?=[^\\{\\}]*\\{[^\\{\\}]*\\}|[^\\{\\}]+$)", std::regex::ECMAScript);
    std::regex Parser::re_param_ = std::regex("^([a-z]*):\\s(.*)$", std::regex::ECMAScript);

    std::shared_ptr<Command> Parser::ParseLine(std::string &line)
    {
        std::shared_ptr<paint::Command> command;
        std::smatch match;
        if (std::regex_match(line, match, Parser::re_load_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_save_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_color_))
        {
            command = std::make_shared<paint::ColorCommand>(std::move(std::shared_ptr<paint::Color>(new paint::ColorRGB888(0, 0, 0))));
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

        throw paint::parse_error(line);
    }
}