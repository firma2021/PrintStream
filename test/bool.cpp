#include "../PrintStream.hpp"

int main()
{
    const int i {1};
    const volatile double d {3.14};
    float f {2023.10f};

    gout << i << '\n';
    gout.int_base(2) << i << '\n';
    gout.show_int_base(false) << i << '\n';
    gout << d << '\n';
    gout << f << '\n';

    const bool tr {true};
    volatile bool fa {false};

    gout << boolalpha;
    gout << tr << '\n';
    gout << fa << '\n';
}
