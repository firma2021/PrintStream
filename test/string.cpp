#include "../PrintStream.hpp"

int main()
{
    string s = "hello, world!";
    string_view sv = s;

    gout << s << '\n';
    gout << std::quoted(s) << '\n';
    gout << sv << '\n';
    gout << s.c_str() << '\n';
}
