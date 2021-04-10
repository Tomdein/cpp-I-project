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
    std::regex Parser::re_line_ = std::regex("^LINE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?}$", std::regex::ECMAScript);
    std::regex Parser::re_circle_ = std::regex("^CIRCLE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?$", std::regex::ECMAScript);
    std::regex Parser::re_bucket_ = std::regex("^BUCKET\\s(%|PX)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?$", std::regex::ECMAScript);
    std::regex Parser::re_resize_ = std::regex("^RESIZE\\s(%|PX)\\s(\\d+)\\s(\\d+)$", std::regex::ECMAScript);
    std::regex Parser::re_rotate_ = std::regex("^ROTATE\\s(CLOCK|COUNTERCLOCK)$", std::regex::ECMAScript);
    std::regex Parser::re_invert_colors_ = std::regex("^INVERTCOLORS$", std::regex::ECMAScript);
    std::regex Parser::re_grayscale_ = std::regex("^GRAYSCALE$", std::regex::ECMAScript);
    std::regex Parser::re_crop_ = std::regex("^CROP\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)$", std::regex::ECMAScript);
    std::regex Parser::re_undo_ = std::regex("^UNDO$", std::regex::ECMAScript);
    std::regex Parser::re_redo_ = std::regex("^REDO$", std::regex::ECMAScript);
    std::regex Parser::re_param_delim_ = std::regex(",\\s(?=[^\\{\\}]*\\{[^\\{\\}]*\\}|[^\\{\\}]+$)", std::regex::ECMAScript);
    std::regex Parser::re_param_ = std::regex("^([a-z-]+):\\s([a-z0-9-]+|\\{.+\\})$", std::regex::ECMAScript);

    std::shared_ptr<Command> Parser::ParseLine(std::string &line)
    {
        std::shared_ptr<Command> command;
        std::smatch match;
        if (std::regex_match(line, match, Parser::re_load_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_save_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_color_))
        {
            command = std::make_shared<ColorCommand>(std::move(std::shared_ptr<Color>(new ColorRGB888(0, 0, 0))));
            return command;
        }
        else if (std::regex_match(line, match, Parser::re_line_))
        {
        }
        else if (std::regex_match(line, match, Parser::re_circle_))
        {
            return std::make_shared<CircleCommand>();
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

        throw parse_error(line);
    }

    std::vector<std::pair<std::string, std::string>> Parser::ParseOptionalArgs(std::string &opt_args)
    {
        std::vector<std::pair<std::string, std::string>> parsed_args;
        std::smatch match;

        std::sregex_token_iterator it(opt_args.begin(), opt_args.end(), Parser::re_param_delim_, -1);
        std::sregex_token_iterator it_end = std::sregex_token_iterator();

        std::string s;
        for (; it != it_end; it++)
        {
            s = it->str();
            std::regex_match(s, match, Parser::re_param_);

            if (match.size() != 3)
            {
                throw parse_error(s);
            }

            parsed_args.emplace_back(match[1], match[2]);
        }

        return parsed_args;
    }
}