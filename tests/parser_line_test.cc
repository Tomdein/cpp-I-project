#include <sstream>
#include <exception>
#include <random>

#include <gtest/gtest.h>

#include "parser.h"

TEST(parser_line_command, valid_commands)
{
     std::random_device rd;                                 // obtain a random number from hardware
     std::mt19937 gen(rd());                                // seed the generator
     std::uniform_int_distribution<> distr_color(0, 255);   // define the range
     std::uniform_int_distribution<> distr_unit(1, 100000); // define the range

     std::stringstream line;

     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen);
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     line.str(std::string());
     line << "LINE % " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen);
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " {width: " << distr_unit(gen) << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     line.str(std::string());
     line << "LINE % " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " {width: " << distr_unit(gen) << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     std::stringstream color;

     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " {color: " << color.str() << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());

     line << "LINE % " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " {color: " << color.str() << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {color: " << color.str() << ", width: " << distr_unit(gen) << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE % " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {color: " << color.str() << ", width: " << distr_unit(gen) << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";

     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE % " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_NO_THROW(paint::Parser::ParseLine(line.str())) << "Failed to parse valid command '" << line.str() << "'";
}

TEST(parser_line_command, invalid_commands)
{
     std::random_device rd;                                    // obtain a random number from hardware
     std::mt19937 gen(rd());                                   // seed the generator
     std::uniform_int_distribution<int> distr_color(0, 256);   // define the range
     std::uniform_int_distribution<int> distr_unit(1, 100000); // define the range

     std::stringstream line;
     std::stringstream color;

     // LIsE
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line << "LIsE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (invalid name) '" << line.str() << "'";

     // lINE
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "lINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (lowercase) '" << line.str() << "'";

     // spaces
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINEPX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (spaces) '" << line.str() << "'";

     // spaces
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX" << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (spaces) '" << line.str() << "'";

     // spaces
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << "" << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (spaces) '" << line.str() << "'";

     // spaces
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << "" << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (spaces) '" << line.str() << "'";

     // spaces
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << "" << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (spaces) '" << line.str() << "'";

     // value
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX "
          << "1A"
          << " " << distr_unit(gen) << " " << distr_unit(gen) << "" << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (value) '" << line.str() << "'";

     // value
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " "
          << "8C"
          << " " << distr_unit(gen) << "" << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (value) '" << line.str() << "'";

     // value
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " "
          << "8caet"
          << "" << distr_unit(gen)
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (value) '" << line.str() << "'";

     // value
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << ""
          << "46daw684"
          << " {width: " << distr_unit(gen) << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (value) '" << line.str() << "'";

     // value
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << "{width: "
          << "1a"
          << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (value) '" << line.str() << "'";

     // zero width
     color.str(std::string());
     color << "{r: " << distr_color(gen) << ", g: " << distr_color(gen) << ", b: " << distr_color(gen) << "}";
     line.str(std::string());
     line << "LINE PX " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen) << " " << distr_unit(gen)
          << " {width: "
          << "0"
          << ", color: " << color.str() << "}";
     EXPECT_THROW(paint::Parser::ParseLine(line.str()), paint::parse_error) << "Parsed invalid command (width is zero) '" << line.str() << "'";
}

int main(int argc, char *argv[])
{
     ::testing::InitGoogleTest(&argc, argv);
     ::testing::GTEST_FLAG(repeat) = 100;
     return RUN_ALL_TESTS();
}