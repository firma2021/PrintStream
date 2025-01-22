#include "../PrintStream.hpp"

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

struct Foo
{
    int a {};
    int b {};
    Foo() = default;
    Foo(int a, int b) : a {a}, b {b} {}
    bool operator<(const Foo& other) const { return other.a < a && other.b < b; }
    bool operator==(const Foo& other) const { return other.a == a && other.b == b; }
    friend ostream& operator<<(ostream& os, const Foo& foo) { return os << foo.a + foo.b; }
};
void test_map()
{
    const multimap<int, int> a {
        {1, 2},
        {3, 4},
        {1, 2},
        {3, 4}
    };

    map<string, int> b {
        {"hello", 2},
        {"world", 3}
    };

    const unordered_multimap<string, vector<string>> c {
        { "first"s,      {"f", "i", "r", "s", "t"}},
        {"second"s, {"s", "e", "c", "o", "n", "d"}},
        { "third"s,      {"t", "h", "r", "i", "d"}}
    };

    auto hash_to_foo = [](const Foo& f) { return hash<int> {}(f.a) + hash<int> {}(f.b); };

    const map<Foo, int> d {
        {Foo(1, 2), 1},
        {Foo(3, 4), 2},
        {Foo(1, 2), 3},
        {Foo(3, 4), 4}
    };

    const unordered_map<Foo, int, decltype(hash_to_foo)> e {
        {Foo(1, 2), 1},
        {Foo(3, 4), 2},
        {Foo(1, 2), 3},
        {Foo(3, 4), 4}
    };

    gout << map<int, string> {} << '\n';
    gout << a << '\n';
    gout << b << '\n';
    gout << c << '\n';
    gout << d << '\n';
    gout << e << '\n';
}

void test_set()
{
    const set a {1, 2, 3};
    const unordered_multiset b {2, 0, 2, 3};
    const multiset c {
        pair {1, 3.14},
        pair {1, 3.14}
    };
    set<Foo> d {
        Foo {1, 2},
        Foo {3, 4}
    };
    auto hash_to_pfoo = [](const pair<Foo, Foo>& f)
    { return f.first.a + f.first.b + f.second.a + f.second.b; };
    unordered_multiset<pair<Foo, Foo>, decltype(hash_to_pfoo)> e;
    e.insert(pair<Foo, Foo> {
        Foo {1, 2},
        Foo {2, 1}
    });
    e.insert(pair<Foo, Foo> {
        Foo {3, 4},
        Foo {4, 3}
    });
    e.insert(pair<Foo, Foo> {
        Foo {5, 6},
        Foo {6, 5}
    });

    gout << set<int> {} << '\n';
    gout << a << '\n';
    gout << b << '\n';
    gout << c << '\n';
    gout << d << '\n';
    gout << e << '\n';
}

int main()
{
    test_map();
    test_set();
}
