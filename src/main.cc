#include <iostream>
#include <string>
#include <memory>
#include <regex>
#include <random>

#include <typeinfo>

#include "parser.h"
#include "colors.h"
#include "paint.h"

std::regex re_param_color_ = std::regex("^r:\\s(\\d{1,3}),\\sg:\\s(\\d{1,3}),\\sb:\\s(\\d{1,3})$", std::regex::ECMAScript);

int main()
{
}