#include "../PrintStream.hpp"

#include <cstdio>

void test() {}

int main()
{
    cout << gout.type_name(main) << '\n';
    cout << gout.type_name(test) << '\n';

    gout << stdin << '\n';
    gout << stdout << '\n';
    gout << stderr << '\n';

    int a {123};
    const volatile int* const volatile p0 = &a; // const volatile int* const volatile: 123
    const volatile int* p1 = &a;                // const volatile int*: 123
    int* const volatile p2 = &a;                // int* const volatile at 0x7ffceeb3841c: 123
    const volatile void* const volatile p3 = &a;
    gout << &a << '\n';
    gout << p0 << '\n';
    gout << p1 << '\n';
    gout << p2 << '\n';
    gout << p3 << '\n';

    gout << nullptr << '\n';
    gout << NULL << '\n';
}
