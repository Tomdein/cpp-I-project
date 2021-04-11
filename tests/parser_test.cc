#include <exception>

#include <gtest/gtest.h>

#include "parser.h"

TEST(parser_error, throw_exception)
{

    ASSERT_THROW(throw paint::parse_error("ASDFGHJKL"), paint::parse_error);
    ASSERT_THROW(throw paint::parse_error(""), std::exception);

    try
    {
        throw paint::parse_error("ASDFGHJKL");
    }
    catch (paint::parse_error &e)
    {
        ASSERT_STREQ("ASDFGHJKL", e.error_substring());
    }
}

TEST(parser, parse_optional_args)
{
    paint::Parser p;
    std::string s;
    std::vector<std::pair<std::string, std::string>> args;

    s = "a: 1";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(1, args.size());
    EXPECT_STREQ("a", args[0].first.c_str());
    EXPECT_STREQ("1", args[0].second.c_str());

    s = "a: 1, b: 2";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(2, args.size());
    EXPECT_STREQ("a", args[0].first.c_str());
    EXPECT_STREQ("1", args[0].second.c_str());
    EXPECT_STREQ("b", args[1].first.c_str());
    EXPECT_STREQ("2", args[1].second.c_str());

    s = "a: 1, b: 2, c: 3";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(3, args.size());
    EXPECT_STREQ("a", args[0].first.c_str());
    EXPECT_STREQ("1", args[0].second.c_str());
    EXPECT_STREQ("b", args[1].first.c_str());
    EXPECT_STREQ("2", args[1].second.c_str());
    EXPECT_STREQ("c", args[2].first.c_str());
    EXPECT_STREQ("3", args[2].second.c_str());

    s = "a: 1, b: 2, c: 3, d: 4";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(4, args.size());
    EXPECT_STREQ("a", args[0].first.c_str());
    EXPECT_STREQ("1", args[0].second.c_str());
    EXPECT_STREQ("b", args[1].first.c_str());
    EXPECT_STREQ("2", args[1].second.c_str());
    EXPECT_STREQ("c", args[2].first.c_str());
    EXPECT_STREQ("3", args[2].second.c_str());
    EXPECT_STREQ("d", args[3].first.c_str());
    EXPECT_STREQ("4", args[3].second.c_str());

    s = "a:1, b: 2";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing space)";

    s = "a: 1,b: 2";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing space)";

    s = "a: 1, b:2";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing space)";

    s = "a: 1 b: 2";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing colon)";

    s = "a: , b: 2";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing value)";

    s = "a: 1, b: ";
    EXPECT_THROW(p.ParseOptionalArgs(s), paint::parse_error) << "Invalid optional args passed (missing value)";

    s = "border-color: {r: 0, g: 10, b: 255}, fill-color: {r: 0, g: 10, b: 255}, border-width: 10";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(3, args.size());
    EXPECT_STREQ("border-color", args[0].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[0].second.c_str());
    EXPECT_STREQ("fill-color", args[1].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[1].second.c_str());
    EXPECT_STREQ("border-width", args[2].first.c_str());
    EXPECT_STREQ("10", args[2].second.c_str());

    s = "fill-color: {r: 0, g: 10, b: 255}, border-color: {r: 0, g: 10, b: 255}, border-width: 10";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(3, args.size());
    EXPECT_STREQ("fill-color", args[0].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[0].second.c_str());
    EXPECT_STREQ("border-color", args[1].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[1].second.c_str());
    EXPECT_STREQ("border-width", args[2].first.c_str());
    EXPECT_STREQ("10", args[2].second.c_str());

    s = "fill-color: {r: 0, g: 10, b: 255}, border-width: 10, border-color: {r: 0, g: 10, b: 255}";
    ASSERT_NO_THROW(args = p.ParseOptionalArgs(s)) << "Failed to parse optional args";
    ASSERT_EQ(3, args.size());
    EXPECT_STREQ("fill-color", args[0].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[0].second.c_str());
    EXPECT_STREQ("border-width", args[1].first.c_str());
    EXPECT_STREQ("10", args[1].second.c_str());
    EXPECT_STREQ("border-color", args[2].first.c_str());
    EXPECT_STREQ("r: 0, g: 10, b: 255", args[2].second.c_str());
}

TEST(parser, parse_circle)
{
    paint::Parser p;
    std::string s;
    std::shared_ptr<paint::Command> command;

    s = "CIRCLE PX 10 10 100";
    EXPECT_NO_THROW(p.ParseLine(s)) << "Failed to parse basic CircleCommand";

    s = "CIRCLE % 10 10 100";
    EXPECT_NO_THROW(p.ParseLine(s)) << "Failed to parse basic CircleCommand";

    s = "CIRCLE PX 10 10 100 {fill: true}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse optional parameter 'fill: true' of CircleCommand";

    s = "CIRCLE PX 10 10 100 {fill: false}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse optional parameter 'fill: false' of CircleCommand";

    s = "CIRCLE PX 10 10 100 {fill-color: {r: 0, g: 10, b: 255}}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse optional parameter 'fill-color' of CircleCommand";

    s = "CIRCLE PX 10 10 100 {border-color: {r: 0, g: 10, b: 255}}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse optional parameter 'border-color' of CircleCommand";

    s = "CIRCLE PX 10 10 100 {border-width: 10}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse optional parameter 'border-width' of CircleCommand";

    s = "CIRCLE PX 10 10 100 {border-width: 10, border-color: {r: 0, g: 10, b: 255}}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse mixed optional parameters of CircleCommand";

    s = "CIRCLE PX 10 10 100 {border-color: {r: 0, g: 10, b: 255}, fill-color: {r: 0, g: 10, b: 255}, border-width: 10}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse mixed optional parameters of CircleCommand";

    s = "CIRCLE PX 10 10 100 {fill: false, fill-color: {r: 0, g: 10, b: 255}, border-width: 10, border-color: {r: 0, g: 10, b: 255}}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse mixed optional parameters of CircleCommand";

    s = "CIRCLE PX 10 10 100 {fill-color: {r: 0, g: 10, b: 255}, fill: false, border-width: 10, border-color: {r: 0, g: 10, b: 255}}";
    ASSERT_NO_THROW(p.ParseLine(s)) << "Failed to parse mixed optional parameters of CircleCommand";

    s = "circle PX 10 10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (lowercase): " << s;

    s = "CIRCLE px 10 10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (lowercase): " << s;

    s = "CIRCLE PX 10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (not enough parameters): " << s;

    s = "CIRCLE PX 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (not enough parameters): " << s;

    s = "CIRCLE PX";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (not enough parameters): " << s;

    s = "CIRCLE";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (not enough parameters): " << s;

    s = "CIRCLE PX 10 10  100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (too many spaces): " << s;

    s = "CIRCLE PX 10  10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (too many spaces): " << s;

    s = "CIRCLE PX  10 10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (too many spaces): " << s;

    s = "CIRCLE  PX 10 10 100";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (too many spaces): " << s;

    s = "CIRCLE PX 10 10 100 {}";
    EXPECT_THROW(p.ParseLine(s), paint::parse_error) << "Invalid CircleCommand passed (empty optional args): " << s;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}