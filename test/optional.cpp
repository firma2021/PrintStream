#include "../PrintStream.hpp"

int main()
{
    const optional<string> a {};
    optional<map<int, string>> b {
        map<int, string> {{1, "hello"s}, {2, "world"s}}
    };

    gout << nullopt << '\n';
    gout << a << '\n';
    gout << b << '\n';
}
