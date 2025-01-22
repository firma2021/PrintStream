#include "../PrintStream.hpp"

int main()
{
    int i {2025};

    gout << i << '\n';
    gout.show_int_base().int_base(2) << i << '\n';
    gout.int_base(8) << i << '\n';
    gout.int_base(16) << i << '\n';
}
