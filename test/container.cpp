#include "../PrintStream.hpp"

#include <deque>
#include <forward_list>
#include <list>
#include <vector>

int main()
{
    const vector a {1, 2, 3, 4, 5, 6, 7, 8, 9};
    forward_list<string> b {"hello", "world", "!"};
    list c {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const deque<list<pair<string, int>>> d {
        {{"hello"s, 1}, {"world"s, 2}},
        { {"fuck"s, 3},  {"life"s, 4}}
    };

    gout << a << '\n';
    gout << std::move(b) << '\n';
    gout << c << '\n';
    gout << d << '\n';
    gout << vector<int> {} << '\n';

    gout.max_print_count(4);
    int arr[] = {1, 2, 3, 4, 5, 6};
    gout << arr << '\n';

    gout.max_print_count(10);
    gout.print_size_threshold(6);
    gout << arr << '\n';
}
