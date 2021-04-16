#include <iostream>
#include <string>
#include <memory>
#include <regex>
#include <random>

#include <typeinfo>

#include "parser.h"
#include "color.h"
#include "paint.h"

std::regex re_param_color_ = std::regex("^r:\\s(\\d{1,3}),\\sg:\\s(\\d{1,3}),\\sb:\\s(\\d{1,3})$", std::regex::ECMAScript);

int main()
{

    std::shared_ptr<paint::Color> sp = std::make_shared<paint::ColorRGB888>(41, 0, 0);
    std::shared_ptr<paint::Color> sp1 = std::make_shared<paint::ColorRGB565>(0, 0, 0);
    sp1->SetColor(*sp);
    paint::ColorRGB565 c(*sp);

    std::cout << (typeid(c).hash_code() == typeid(paint::ColorRGB565).hash_code()) << '\n';

    paint::Parser p;
    std::string s("CIRCLE PX 10 10 100");

    std::smatch match;
    std::regex re_circle_ = std::regex("^CIRCLE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)(:?\\s\\{(.*)\\})?$", std::regex::ECMAScript);
    [[maybe_unused]] bool matched = std::regex_match(s, match, re_circle_);

    //std::shared_ptr<paint::Command> command = p.ParseLine(s);

    std::vector<std::pair<std::string, std::string>> args;
    //s = "border-color: {r: 0, g: 10, b: 255}, fill-color: {r: 0, g: 10, b: 255}, border-width: 10";
    s = "border-width: 10";
    args = p.ParseOptionalArgs(s);

    paint::Parser::ParseLine("LINE PX 1 2 3 4");
    paint::Parser::ParseLine("LINE PX 1 2 3 4 {width: 50, color: {r: 0, g: 5, b: 255}}");

    s = "r: 1, g: 10, b: 500";
    std::regex_match(s, match, re_param_color_);

    std::cout << match[1].str() << ", " << match[2].str() << ", " << match[3].str() << std::endl;

    int r = std::stoi(match[1].str());
    int g = std::stoi(match[2].str());
    int b = std::stoi(match[3].str());

    std::cout << r << ", " << g << ", " << b << std::endl;

    std::cout << re_param_color_.mark_count() << ", " << match.size() << std::endl;

    paint::Parser::ParseLine("CIRCLE PX 1 2 3 {fill-color: {r: 0, g: 5, b: 255}}");

    paint::DataPixels dp(paint::Point{10, 10}, std::unique_ptr<paint::Color>(new paint::ColorRGB888()));
    dp.GetColorType();

    paint::ColorRGB565 color_inverse(0, 62, 5);
    color_inverse.InvertColor();
}