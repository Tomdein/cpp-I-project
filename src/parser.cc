#include <string>
#include <memory>
#include <regex>
#include <iostream>
#include <filesystem>
#include <vector>
#include <utility>

#include "unit.h"
#include "point.h"
#include "rotation.h"
#include "command.h"
#include "parser.h"

namespace paint
{
    const char *parse_error::what() const noexcept
    {
        return "Error while parsing line";
    }

    const char *parse_error::error_substring() const noexcept
    {
        return error_substring_;
    }

    std::regex Parser::re_save_ = std::regex("^SAVE\\s([a-zA-Z0-9\\s\\\\/._-]+)$");
    std::regex Parser::re_load_ = std::regex("^LOAD\\s([a-zA-Z0-9\\s\\\\/._-]+)$");
    std::regex Parser::re_color_ = std::regex("^COLOR\\s(\\d{1,3})\\s(\\d{1,3})\\s(\\d{1,3})$");
    std::regex Parser::re_line_ = std::regex("^LINE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?$");
    std::regex Parser::re_circle_ = std::regex("^CIRCLE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?$");
    std::regex Parser::re_bucket_ = std::regex("^BUCKET\\s(%|PX)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.+)\\})?$");
    std::regex Parser::re_resize_ = std::regex("^RESIZE\\s(%|PX)\\s(\\d+)\\s(\\d+)$");
    std::regex Parser::re_rotate_ = std::regex("^ROTATE\\s(CLOCK|COUNTERCLOCK)$");
    std::regex Parser::re_invert_colors_ = std::regex("^INVERTCOLORS$");
    std::regex Parser::re_grayscale_ = std::regex("^GRAYSCALE$");
    std::regex Parser::re_crop_ = std::regex("^CROP\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)$");
    std::regex Parser::re_undo_ = std::regex("^UNDO$");
    std::regex Parser::re_redo_ = std::regex("^REDO$");
    std::regex Parser::re_param_delim_ = std::regex(",\\s(?=[^\\{\\}]*\\{[^\\{\\}]*\\}|[^\\{\\}]+$)");
    std::regex Parser::re_param_ = std::regex("^([a-z-]+):\\s(?:([a-z0-9-]+)|\\{(.+)\\})$");
    std::regex Parser::re_param_color_ = std::regex("^r:\\s(\\d{1,3}),\\sg:\\s(\\d{1,3}),\\sb:\\s(\\d{1,3})$");

    std::shared_ptr<Command> Parser::ParseLine(const std::string &line)
    {
        std::shared_ptr<Command> command;
        std::smatch match;

        // LOAD command
        // Do not check the file
        if (std::regex_match(line, match, Parser::re_load_))
        {
            command = std::make_shared<SaveCommand>(std::filesystem::path(match[1].str()));
        }
        // SAVE command
        // Do not check the file
        else if (std::regex_match(line, match, Parser::re_save_))
        {
            command = std::make_shared<SaveCommand>(std::filesystem::path(match[1].str()));
        }
        // COLOR command
        else if (std::regex_match(line, match, Parser::re_color_))
        {
            int r = std::stoi(match[1].str());
            int g = std::stoi(match[2].str());
            int b = std::stoi(match[3].str());

            if (r > 255 || g > 255 || b > 255)
            {
                throw parse_error(line);
            }

            command = std::make_shared<ColorCommand>(std::shared_ptr<Color>(new ColorRGB888(r, g, b)));
        }
        // LINE command
        else if (std::regex_match(line, match, Parser::re_line_))
        {
            // Using relative units
            std::shared_ptr<LineCommand> line_command;
            if (match[1].str() == "%")
            {
                line_command = std::make_shared<LineCommand>(std::make_shared<PointPer>(std::stof(match[2].str()), std::stof(match[3].str())),
                                                             std::make_shared<PointPer>(std::stof(match[4].str()), std::stof(match[5].str())));
            }
            // Using PX units
            else
            {
                line_command = std::make_shared<LineCommand>(std::make_shared<PointPX>(std::stof(match[2].str()), std::stof(match[3].str())),
                                                             std::make_shared<PointPX>(std::stof(match[4].str()), std::stof(match[5].str())));
            }

            // Has optional parameters
            if (match[7].matched == true)
            {
                std::vector<std::pair<std::string, std::string>> opt_args = Parser::ParseOptionalArgs(match[7].str());

                bool has_color = false;
                bool has_line_width = false;
                for (auto &[arg, val] : opt_args)
                {
                    if (arg == "color" && has_color == false)
                    {
                        has_color = true;

                        line_command->AddLineColor(std::make_shared<ColorRGB888>(std::move(ParseColorVal(val))));
                    }
                    else if (arg == "width" && has_line_width == false)
                    {
                        int width = std::stoi(val);
                        if (width == 0)
                        {
                            throw parse_error(val);
                        }

                        has_line_width = true;
                        line_command->AddLineWidth(width);
                    }
                    else
                    {
                        throw parse_error(match[7].str());
                    }
                }
            }

            command = std::move(line_command);
        }
        // CIRCLE command
        else if (std::regex_match(line, match, Parser::re_circle_))
        {
            std::shared_ptr<CircleCommand> circle_command;

            // Using relative units
            if (match[1].str() == "%")
            {
                circle_command = std::make_shared<CircleCommand>(std::make_shared<PointPer>(std::stoi(match[2].str()), std::stoi(match[3].str())),
                                                                 std::stoi(match[4].str()));
            }
            // Using PX units
            else
            {
                circle_command = std::make_shared<CircleCommand>(std::make_shared<PointPX>(std::stoi(match[2].str()), std::stoi(match[3].str())),
                                                                 std::stoi(match[4].str()));
            }

            // Has optional parameters
            if (match[6].matched == true)
            {
                std::vector<std::pair<std::string, std::string>> opt_args = Parser::ParseOptionalArgs(match[6].str());

                bool has_fill_arg = false;
                bool has_fill_color_arg = false;
                bool has_border_color_arg = false;
                bool has_border_width_arg = false;

                for (auto &[arg, val] : opt_args)
                {
                    if (arg == "fill" && !has_fill_arg)
                    {
                        has_fill_arg = true;

                        if (val == "true")
                        {
                            circle_command->SetCircleFill(true);
                        }
                        else if (val == "false")
                        {
                            circle_command->SetCircleFill(false);
                        }
                        else
                        {
                            throw parse_error(arg + ": " + val);
                        }
                    }
                    else if (arg == "fill-color" && !has_fill_color_arg)
                    {
                        has_fill_color_arg = true;
                        circle_command->AddCircleFillColor(std::make_shared<ColorRGB888>(Parser::ParseColorVal(val)));
                    }
                    else if (arg == "border-color" && !has_border_color_arg)
                    {
                        has_border_color_arg = true;
                        circle_command->AddCircleBorderColor(std::make_shared<ColorRGB888>(Parser::ParseColorVal(val)));
                    }
                    else if (arg == "border-width" && !has_border_width_arg)
                    {
                        has_border_width_arg = true;
                        circle_command->AddCircleBorderWidth(std::stoi(val));
                    }
                    else
                    {
                        throw parse_error(arg + ": " + val);
                    }
                }
            }

            command = circle_command;
        }
        // BUCKET command
        else if (std::regex_match(line, match, Parser::re_bucket_))
        {

            std::shared_ptr<BucketCommand> bucket_command;
            // Using relative units
            if (match[1].str() == "%")
            {
                bucket_command = std::make_shared<BucketCommand>(std::make_shared<PointPer>(std::stof(match[2].str()), std::stof(match[3].str())));
            }
            // Using PX units
            else
            {
                bucket_command = std::make_shared<BucketCommand>(std::make_shared<PointPX>(std::stof(match[2].str()), std::stof(match[3].str())));
            }

            // Has optional parameters
            if (match[5].matched == true)
            {
                std::vector<std::pair<std::string, std::string>> opt_args = Parser::ParseOptionalArgs(match[5].str());

                for (auto &[arg, val] : opt_args)
                {
                    bool has_color_arg = false;

                    if (arg == "color" && !has_color_arg)
                    {
                        has_color_arg = true;
                        bucket_command->AddFillColor(std::make_shared<ColorRGB888>(Parser::ParseColorVal(val)));
                    }
                    else
                    {
                        throw parse_error(arg + ": " + val);
                    }
                }
            }
        }
        // RESIZE command
        else if (std::regex_match(line, match, Parser::re_resize_))
        {
            std::shared_ptr<ResizeCommand> resize_command;

            if (match[1].str() == "%")
            {
                resize_command = std::make_shared<ResizeCommand>(std::make_shared<PointPer>(std::stof(match[2].str()),
                                                                                            std::stof(match[3].str())));
            }
            else if (match[1].str() == "PX")
            {
                resize_command = std::make_shared<ResizeCommand>(std::make_shared<PointPX>(std::stoi(match[2].str()),
                                                                                           std::stoi(match[3].str())));
            }
            command = std::move(resize_command);
        }
        // ROTATE command
        else if (std::regex_match(line, match, Parser::re_rotate_))
        {
            std::shared_ptr<RotateCommand> rotate_command;

            if (match[1].str() == "CLOCK")
            {
                rotate_command = std::make_shared<RotateCommand>(Rotation::kClock);
            }
            else
            {
                rotate_command = std::make_shared<RotateCommand>(Rotation::kCounterClock);
            }
        }
        // INVERT_COLOR command
        else if (std::regex_match(line, match, Parser::re_invert_colors_))
        {
            command = std::make_shared<InvertColorCommand>();
        }
        // GRAYSCALE command
        else if (std::regex_match(line, match, Parser::re_grayscale_))
        {
            command = std::make_shared<GrayscaleCommand>();
        }
        // CROP command
        else if (std::regex_match(line, match, Parser::re_crop_))
        {
            // Using relative units
            if (match[1].str() == "%")
            {
                command = std::make_shared<CropCommand>(std::make_shared<PointPer>(std::stoi(match[2].str()), std::stoi(match[3].str())),
                                                        std::make_shared<PointPer>(std::stoi(match[4].str()), std::stoi(match[5].str())));
            }
            // Using PX units
            else
            {
                command = std::make_shared<CropCommand>(std::make_shared<PointPX>(std::stoi(match[2].str()), std::stoi(match[3].str())),
                                                        std::make_shared<PointPX>(std::stoi(match[4].str()), std::stoi(match[5].str())));
            }
        }
        // UNDO command
        else if (std::regex_match(line, match, Parser::re_undo_))
        {
            command = std::make_shared<UndoCommand>();
        }
        // REDO command
        else if (std::regex_match(line, match, Parser::re_redo_))
        {
            command = std::make_shared<RedoCommand>();
        }
        else
        {
            throw parse_error(line);
        }

        return command;
    }

    std::vector<std::pair<std::string, std::string>> Parser::ParseOptionalArgs(const std::string &opt_args)
    {
        std::vector<std::pair<std::string, std::string>> parsed_args;
        std::smatch match;

        std::sregex_token_iterator it(opt_args.begin(), opt_args.end(), Parser::re_param_delim_, -1);
        std::sregex_token_iterator it_end = std::sregex_token_iterator();

        std::string s;
        for (; it != it_end; it++)
        {
            s = it->str();

            // If failed to parse -> throw
            if (!std::regex_match(s, match, Parser::re_param_))
            {
                throw parse_error(s);
            }

            // Concatenate match[2] & match[3]. One of the will be always empty
            parsed_args.emplace_back(match[1].str(), match[2].str() + match[3].str());
        }

        return parsed_args;
    }

    ColorRGB888 Parser::ParseColorVal(const std::string &color_arg)
    {
        std::smatch color_match;
        std::regex_match(color_arg, color_match, Parser::re_param_color_);

        // Could not match the color
        if (color_match.empty())
        {
            throw parse_error(color_arg);
        }

        int r = std::stoi(color_match[1].str());
        int g = std::stoi(color_match[2].str());
        int b = std::stoi(color_match[3].str());

        if (r > 255 || g > 255 || b > 255)
        {
            throw parse_error(color_arg);
        }

        return ColorRGB888(r, g, b);
    }
}