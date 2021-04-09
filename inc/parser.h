#ifndef _H_PAINT_INC_PARSER_H
#define _H_PAINT_INC_PARSER_H

#include <filesystem>
#include <regex>
#include <exception>

#include "command.h"

namespace paint
{
    class parse_error : public std::exception
    {
    public:
        parse_error(const char *what_arg, const char *error_substring);
        parse_error(const std::string &what_arg, const std::string &error_substring);

        virtual const char *what() const noexcept override;
        virtual const char *error_substring() const noexcept;

    private:
        char error_substring_[256];
    };

    class Parser
    {
    public:
        Parser() = default;

        static std::shared_ptr<Command> ParseLine(std::string &line);

    private:
        static std::regex re_save_;
        static std::regex re_load_;
        static std::regex re_color_;
        static std::regex re_line_;
        static std::regex re_circle_;
        static std::regex re_bucket_;
        static std::regex re_resize_;
        static std::regex re_rotate_;
        static std::regex re_invert_colors_;
        static std::regex re_grayscale_;
        static std::regex re_crop_;
        static std::regex re_undo_;
        static std::regex re_redo_;
        static std::regex re_param_delim_;
        static std::regex re_param_;
    };

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

    // class FileParser
    // {
    // public:
    //     FileParser(FileParser &other) = delete;
    //     void operator=(FileParser const &) = delete;

    //     static FileParser &getInstance()
    //     {
    //         static FileParser instance;
    //         return instance;
    //     }

    //     bool ParseFile();

    // private:
    //     FileParser() {}
    // };

    // class CLIParser
    // {
    // public:
    //     CLIParser(CLIParser &other) = delete;
    //     void operator=(CLIParser const &) = delete;

    //     static CLIParser &getInstance()
    //     {
    //         static CLIParser instance;
    //         return instance;
    //     }

    //     bool ParseCLI();

    // private:
    //     CLIParser() {}
    // };

}

#endif // _H_PAINT_INC_PARSER_H