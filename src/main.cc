#include <iostream>
#include <string>
#include <memory>

#include <typeinfo>

#include "parser.h"
#include "color.h"

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
    bool matched = std::regex_match(s, match, re_circle_);

    std::shared_ptr<paint::Command> command = p.ParseLine(s);

    std::vector<std::pair<std::string, std::string>> args;
    s = "border-color: {r: 0, g: 10, b: 255}, fill-color: {r: 0, g: 10, b: 255}, border-width: 10";
    args = p.ParseOptionalArgs(s);
}