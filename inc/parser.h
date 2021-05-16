#ifndef _H_PAINT_INC_PARSER_H
#define _H_PAINT_INC_PARSER_H

#include <regex>
#include <exception>
#include <cstring>

#include <gtest/gtest.h>

#include "color.h"
#include "command.h"

namespace paint
{

    /**
     * @brief A exception class used when throwing parser errors.
     * 
     * This object is thrown whenever invalid command while parsing a line with commands.
     * 
     * This class derives from std::exception.
     * 
     */
    class parse_error : public std::exception
    {
    public:
        parse_error(const char *error_substring);

        parse_error(const std::string &error_substring);

        virtual ~parse_error() override{};

        /**
         * @brief Return what caused the exception.
         * 
         * @return const char* message describing what caused the exception.
         */
        virtual const char *what() const noexcept override;
        /**
         * @brief Get the line that caused the exception.
         * 
         * If the line is longer than parse_error::kParseErrorSubstringLength, the string will be truncated.
         * 
         * @return const char* cstring containing the text with the error from command.
         */
        virtual const char *error_substring() const noexcept;

    private:
        /**
         * @brief A constant that define the maximum length of the text.
         * 
         */
        enum
        {
            kParseErrorSubstringLength = 256, /// Maximum length of the substring.
        };

        char error_substring_[kParseErrorSubstringLength]; /// Array storing error command text.
    };

    /**
     * @brief A class used to parse commands.
     * 
     * This object can parse commands line by line and stores them.
     * 
     * If a invalid or unknown command is encountered, Parser::ParseLine() throws a \ref parse_error exception.
     * 
     */
    class Parser
    {
    public:
        Parser() = default;

        /**
         * @brief Parses whole line and returns the Command object
         * 
         * This method throws \ref parse_error exception if line with unknown or invalid command is passed.
         * 
         * @param line a string to parse
         * @return std::shared_ptr<Command> pointer to the parsed command
         */
        static std::shared_ptr<Command> ParseLine(const std::string &line);

        static void RemoveLastCRLF(std::string &line);

    private:
        FRIEND_TEST(parser, parse_optional_args); /// Make the Parser::ParseOptionalArgs() and  Painter::ParseColorVal() accessible to google test.

        /**
         * @brief Returns vector of pairs, that contain oprional args in a pair: <argument>, <value>.
         * 
         * The input is any string with optional arguments (without '{' and ''} brackets) in any order.
         * If unknown or invalid optional argument is encountered, this method throws a \ref parse_error exception.
         * 
         * @param opt_args a string containing optional arguments (without '{' and ''} brackets).
         * @return std::vector<std::pair<std::string, std::string>> a std::vector of std::pair containing <argument>, <value>
         */
        static std::vector<std::pair<std::string, std::string>> ParseOptionalArgs(const std::string &opt_args);
        /**
         * @brief Parses the color argument.
         * 
         * Parses the argument 'color: {r: ..., g: ..., b: ...}' with RGB888 color format.
         * 
         * @param color_arg a string in color format.
         * @return ColorRGB888 the parsed color.
         */
        static ColorRGB888 ParseColorVal(const std::string &color_arg);

        static std::regex re_save_;          /// RegEx for save command.
        static std::regex re_load_;          /// RegEx for load command.
        static std::regex re_color_;         /// RegEx for color command.
        static std::regex re_line_;          /// RegEx for line command.
        static std::regex re_circle_;        /// RegEx for circle command.
        static std::regex re_bucket_;        /// RegEx for bucket command.
        static std::regex re_resize_;        /// RegEx for resize command.
        static std::regex re_rotate_;        /// RegEx for rotate command.
        static std::regex re_invert_colors_; /// RegEx for invercolor command.
        static std::regex re_grayscale_;     /// RegEx for grayscale command.
        static std::regex re_crop_;          /// RegEx for crop command.
        static std::regex re_undo_;          /// RegEx for undo command.
        static std::regex re_redo_;          /// RegEx for redo command.
        static std::regex re_param_delim_;   /// RegEx for tokenizing optional arguments.
        static std::regex re_param_;         /// RegEx for spliting optional argument into <arg>, <value> pair.
        static std::regex re_param_color_;   /// RegEx for parsing RGB888 color.
    };
}

#endif // _H_PAINT_INC_PARSER_H