## 概述
`PrintStream`类是`ostream`对象的包装器。它保存了`ostream`对象的指针。

你可以像使用`ostream`对象一样使用`PrintStream`。

`PrintStream`提供了更方便、更强大的格式化功能，支持绝大多数标准库组件的格式化和自定义类型的格式化，使用被包装的ostream对象进行输出。

## 基本用法
1. 包含PrintStream.hpp
2. 通过`ostream`对象构造`PrintStream`对象，例如：`ostringstream oss; PrintStream print_stream {oss}; `
3. 像使用`ostream`对象那样，使用`print_stream`。

`PrintStream.hpp`中已经提供了`PrintStream gout {cout}`，你可以直接使用`gout`。

## 使用示例

### 打印容器
```cpp
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
```

输出:
```txt
[<size = 9> 1,2,3,4,...,6,7,8,9]
[hello,world,!]
[<size = 9> 1,2,3,4,...,6,7,8,9]
[[(hello 1),(world 2)],[(fuck 3),(life 4)]]
[]
[<size = 6> 1,2,...,5,6]
[1,2,3,4,5,6]
```

当容器中元素的个数大于m时，打印容器中元素的总数。你可以调用`gout.print_size_threshold(m) `来调整m的大小。

当容器中元素的个数大于n时，只打印n个元素，其余的元素省略，并打印容器中元素的总数。你可以调用`gout.max_print_count(n)`来调整n的大小。

### 打印容器适配器
```cpp
#include "../PrintStream.hpp"

#include <queue>
#include <stack>

int main()
{
    vector v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    priority_queue pq {less<> {}, v};
    stack s {v};
    queue q {v};

    gout << pq << '\n';
    gout << s << '\n';
    gout << q << '\n';
}

```

结果为
```txt
[<size = 12> 12,11,7,9,...,4,2,5,3]
[<size = 12> 1,2,3,4,...,9,10,11,12]
[<size = 12> 1,2,3,4,...,9,10,11,12]
```

### 打印tuple
array, pair, tuple，它们能在编译期确定大小，都被视为tuple。

c风格的数组被视为容器。
```cpp
int main()
{
    const tuple<> a;
    const tuple b {1, 3.14, 2023.0F};
    tuple c {make_pair(2.48, "hello"s), make_pair(6.96, "world"s)};
    const array d {1, 2, 3, 4, 5, 6, 7, 8, 9};

    gout << a << '\n';
    gout << b << '\n';
    gout << std::move(c) << '\n';
    gout << d << '\n';
}
```

结果为
```txt
()
(1 3.14 2023)
((2.48 hello) (6.96 world))
(1 2 3 4 5 6 7 8 9)
```

### 打印map和set
包括multi/unordered的map/set
```cpp

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
```

结果为
```txt
{}
{1:2,1:2,3:4,3:4}
{hello:2,world:3}
{second:[s,e,c,o,n,d],third:[t,h,r,i,d],first:[f,i,r,s,t]}
{7:2,3:1}
{7:2,3:1}
{}
{1,2,3}
{3,0,2,2}
{(1 3.14),(1 3.14)}
{7,3}
{(11 11),(7 7),(3 3)}
```

### 打印输入流
```cpp
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
```

结果为

```
input stream with state = good, size = 422, line = 27
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

input stream with state = fail

input stream with state = good, size = 0, line = 0

input stream with state = eof

input stream with state = good, size = 0, line = 0

input stream with state = good, This input stream is not seekable.
```
### 打印其它对象
打印optional, any, variant, smart_pointer, chrono, enum, int, float, iterator等的示例，见test文件夹下的相应测试文件。
