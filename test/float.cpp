#include "../PrintStream.hpp"

int main()
{
    double d {3.1415926535};


    gout << d << '\n';
    gout.float_precision(20) << d << '\n';

    gout.float_fixed() << d << '\n';
    gout.float_scientific() << d << '\n';
    gout.float_hex() << d << '\n';
    gout.float_general() << d << '\n';
}
