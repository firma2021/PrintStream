#include "../PrintStream.hpp"

struct Foo
{
    int i;

    Foo(int i) : i {i} {}
    Foo() = default;
    Foo(const Foo&) = default;
    Foo(Foo&&) noexcept = default;
    Foo& operator=(const Foo&) = default;
    Foo& operator=(Foo&&) noexcept = default;
};

template <typename OutStream, typename T>
    requires requires(OutStream& out, const T& obj) { out << obj; }
void generic_to_string(OutStream& out, const T& obj)
{
    out << obj;
}

int main()
{
    gout << typeid(Foo) << '\n';

    gout << any {nullopt} << '\n';

    any a;
    gout << a << '\n';

    a = vector<string> {"hello,", "world!"};
    gout << a << '\n';

    a = Foo {2025};
    gout << a << '\n';

    const any b;
    gout << b << '\n';
}
