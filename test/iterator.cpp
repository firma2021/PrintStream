#include "../PrintStream.hpp"

#include <deque>
#include <forward_list>
#include <fstream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

int main()
{
    gout << array<int, 3> {}.cbegin() << '\n';

    gout << vector<int> {}.crbegin() << '\n';
    gout << string {}.crbegin() << '\n';
    gout << deque<int> {}.begin() << '\n';
    gout << forward_list<int> {}.begin() << '\n';
    gout << list<int> {}.rbegin() << '\n';
    gout << set<int> {}.cbegin() << '\n';
    gout << multiset<int> {}.rbegin() << '\n';
    gout << unordered_set<int> {}.begin() << '\n';
    gout << unordered_multiset<int> {}.begin() << '\n';
    gout << map<int, int> {}.begin() << '\n';
    gout << multimap<int, int> {}.rbegin() << '\n';
    gout << unordered_map<int, int> {}.begin() << '\n';
    gout << unordered_multimap<int, int> {}.begin() << '\n';
    gout << istreambuf_iterator<char> {cin} << '\n';
    gout << ostreambuf_iterator<char> {cout} << '\n';
    fstream fs;
    gout << istream_iterator<float> {fs} << '\n';
    gout << ostream_iterator<double> {fs, ""} << '\n';

    const vector v {stdin, stdout, stderr};
    gout << prev(v.end()) << '\n';
}
