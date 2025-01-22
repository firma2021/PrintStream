#include "../PrintStream.hpp"

enum class Color : unsigned char
{
    Green,
    Red,
    Blue,
};

enum CColor
{
    Red,
    Green
};

int main()
{
    gout << Color::Green << '\n';
    gout << gout.enum_name<Color, Color::Green>() << '\n';
    gout << gout.enum_name<Color, static_cast<Color>(100)>() << '\n';

    gout << Red << '\n';
    gout << gout.c_enum_name<CColor, Red>() << '\n';
}
