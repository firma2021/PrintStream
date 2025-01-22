#include "../PrintStream.hpp"

#include <fstream>
#include <iterator>
#include <string>

int main()
{
    ifstream fin {"../istream.cpp"};
    gout << fin << '\n';

    int a {};
    fin >> a;
    gout << fin << '\n';
    fin.clear();

    string buf {istreambuf_iterator {fin}, {}};
    gout << fin << '\n';

    fin.peek();
    gout << fin << '\n';

    fin.clear();
    gout << fin << '\n';

    gout << cin << '\n';
}
