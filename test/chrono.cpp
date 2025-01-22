#include "../PrintStream.hpp"

#include <chrono>

int main()
{
    auto beg {chrono::steady_clock::now()};

    auto now {chrono::system_clock::now()};
    gout << now << '\n';

    auto end {chrono::steady_clock::now()};
    auto d = (end - beg);

    gout << d << '\n';
    gout.nanoseconds() << d << '\n';
    gout.microseconds() << d << '\n';
    gout.milliseconds() << d << '\n';
    gout.seconds() << d << '\n';
}
