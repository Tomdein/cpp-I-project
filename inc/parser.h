#ifndef _H_PAINT_INC_PARSER_H
#define _H_PAINT_INC_PARSER_H

#include <regex>
#include <exception>
#include <cstring>

#include "command.h"

namespace paint
{
#define PARSE_ERROR_SUBSTRING_LEN 256

    class parse_error : public std::exception
    {
    public:
        parse_error(const char *error_substring)
        {
            std::strncpy(error_substring_, error_substring, PARSE_ERROR_SUBSTRING_LEN);
            error_substring_[PARSE_ERROR_SUBSTRING_LEN - 1] = '\0';
        }

        parse_error(const std::string &error_substring)
        {
            size_t n = error_substring.copy(error_substring_, PARSE_ERROR_SUBSTRING_LEN);
            if (n != PARSE_ERROR_SUBSTRING_LEN)
            {
                error_substring_[n] = '\0';
            }
            else
            {
                error_substring_[PARSE_ERROR_SUBSTRING_LEN] = '\0';
            }
        }

        virtual ~parse_error() override{};

        virtual const char *what() const noexcept override;
        virtual const char *error_substring() const noexcept;

    private:
        char error_substring_[PARSE_ERROR_SUBSTRING_LEN];
    };

    class Parser
    {
    public:
        Parser() = default;

        static std::shared_ptr<Command> ParseLine(std::string &line);
        static std::vector<std::pair<std::string, std::string>> ParseOptionalArgs(std::string &opt_args);

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
}

#endif // _H_PAINT_INC_PARSER_H