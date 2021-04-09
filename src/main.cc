#include <iostream>
#include <string>
#include <memory>

#include <typeinfo>

#include "image.h"
#include "parser.h"

int main()
{
    std::string s = "CIRCLE PX 1 1 1 {fill: true, aaa, bbb, fill-color: {r: 0, g: 0, b: 0}, bebe, wewe, border-color: {r: 0, g: 0, b: 0}}";
    std::regex re_circle_ = std::regex("^CIRCLE\\s(%|PX)\\s(\\d+)\\s(\\d+)\\s(\\d+)(?:\\s\\{(.*)\\})?$", std::regex::ECMAScript);
    std::regex re_param_ = std::regex(",\\s(?=[^\\{\\}]*\\{[^\\{\\}]*\\}|[^\\{\\}]+$)", std::regex::ECMAScript);

    std::smatch match;
    if (!std::regex_match(s, match, re_circle_))
    {
        std::cout << "Not matched: '" << s << "'" << std::endl;
    }

    for (auto m : match)
    {
        std::cout << m << "\n";
    }
    std::cout << '\n';

    std::cout << match[5] << "\n";
    std::string s_par(match[5]);

    if (!std::regex_search(s_par, match, re_param_))
    {
        std::cout << "Not matched: '" << s << "'" << std::endl;
    }

    std::cout << "SKIP:\n";

    std::sregex_token_iterator it(s_par.begin(), s_par.end(), re_param_, -1);
    std::sregex_token_iterator it_end = std::sregex_token_iterator();

    while (it != it_end)
    {
        std::cout << *it << '\n';
        it++;
    }

    std::cout << "\n\n";
    std::shared_ptr<paint::Color> sp = std::make_shared<paint::ColorRGB888>(41, 0, 0);
    std::shared_ptr<paint::Color> sp1 = std::make_shared<paint::ColorRGB565>(0, 0, 0);
    sp1->SetColor(*sp);
    paint::ColorRGB565 c(*sp);

    std::cout << (typeid(c).hash_code() == typeid(paint::ColorRGB565).hash_code()) << '\n';
}