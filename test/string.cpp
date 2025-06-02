#include "../PrintStream.hpp"
#include <charconv>

int main()
{
    string s = "hello, world!";
    string_view sv = s;

    gout << s << '\n';
    gout << std::quoted(s) << '\n';
    gout << sv << '\n';
    gout << s.c_str() << '\n';

    s = "hello,\nworld!\t\"test\"\\";
    sv = s;
    const char* cstr = s.c_str();

    gout << s << '\n';
    gout << sv << '\n';
    gout << cstr << '\n';


    string empty;
    gout << empty << '\n';

    string ctrl = "\n\t\r";
    gout << ctrl << '\n';

    string special = R"("quoted\backslash")";
    gout << special << '\n';

    string all;
    for (int i = 0; i < 32; ++i) { all += static_cast<char>(i); }
    all += "ABCxyz";
    gout << all << '\n';

    const char* h = "hello, world! \n";
    gout << h << '\n';

    char c_s[] = "hello, world! \n";
    gout << c_s << '\n';

    char cc_s[] = "hello, world! \n";
    gout << cc_s << '\n';
}
