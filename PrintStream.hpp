// generic_to_string
// Created by kiki on 2022/9/25.17:29
#pragma once

#include <algorithm>
#include <any>
#include <array>
#include <charconv>
#include <chrono>
#include <concepts>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <ranges>
#include <source_location>
#include <string>
#include <string_view>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
constexpr bool need_demangle {true};
#else
constexpr bool need_demangle {false};
#endif

using namespace std;

template <typename T>
concept is_int = same_as<T, unsigned short> || same_as<T, unsigned int> ||
                 same_as<T, unsigned long> || same_as<T, unsigned long long> || same_as<T, short> ||
                 same_as<T, int> || same_as<T, long> || same_as<T, long long>;

template <typename OutStream>
    requires is_base_of_v<ostream, OutStream>
class PrintStream
{
    OutStream* out;

    int base {10};
    bool show_base {true};

    int precision {6};
    chars_format float_fmt {chars_format::general};

    size_t size_threshold {8};
    size_t max_print {8};

    bool micro {false};
    bool nano {false};
    bool milli {true};
    bool second {false};

public:
    explicit PrintStream(OutStream& out_) : out {&out_} {}
    OutStream& get() { return *out; }

    PrintStream& print_size_threshold(size_t size)
    {
        size_threshold = size;
        return *this;
    }

    PrintStream& max_print_count(size_t size)
    {
        max_print = size;
        return *this;
    }

    PrintStream& int_base(int b)
    {
        if (b >= 2 && b <= 36) { base = b; }
        return *this;
    }
    PrintStream& show_int_base(bool show = true)
    {
        show_base = show;
        return *this;
    }
    PrintStream& float_precision(int p = 6)
    {
        precision = p;
        return *this;
    }
    PrintStream& float_hex()
    {
        float_fmt = chars_format::hex;
        return *this;
    }
    PrintStream& float_fixed()
    {
        float_fmt = chars_format::fixed;
        return *this;
    }
    PrintStream& float_scientific()
    {
        float_fmt = chars_format::scientific;
        return *this;
    }
    PrintStream& float_general()
    {
        float_fmt = chars_format::general;
        return *this;
    }
    PrintStream& nanoseconds()
    {
        nano = true;
        micro = false;
        milli = false;
        second = false;

        return *this;
    }
    PrintStream& microseconds()
    {
        nano = false;
        micro = true;
        milli = false;
        second = false;

        return *this;
    }
    PrintStream& milliseconds()
    {
        nano = false;
        micro = false;
        milli = true;
        second = false;

        return *this;
    }
    PrintStream& seconds()
    {
        nano = false;
        micro = false;
        milli = false;
        second = true;

        return *this;
    }

public:
    template <typename T>
    static constexpr auto type_name()
    {
        const string_view func_name {source_location::current().function_name()};
        auto begin {func_name.find_first_of('=') + 2};
        auto end {func_name.find_first_of(';')};
        return func_name.substr(begin, end - begin);
    }

    static constexpr auto type_name(auto&& arg) { return type_name<decltype(arg)>(); }

    template <typename T, T t>
    static constexpr auto enum_name()
    {
        // static constexpr auto PrintStream<OutStream>::enum_name() [with T = Color; T t = Color::Green; OutStream = std::basic_ostream<char>]
        const string_view func_name {source_location::current().function_name()};

        auto begin {func_name.find(';') + 8};
        auto end {func_name.find(';', begin)};
        return func_name.substr(begin, end - begin);
    }

    template <typename T, T t>
    static constexpr auto c_enum_name()
    {
        // static constexpr auto PrintStream<OutStream>::c_enum_name() [with T = CColor; T t = Red; OutStream = std::basic_ostream<char>]
        const string_view func_name {source_location::current().function_name()};
        auto name_begin {func_name.find('=') + 2};
        auto name_end {func_name.find(';', name_begin)};
        auto value_begin {func_name.find('=', name_begin) + 2};
        auto value_end {func_name.find(';', value_begin)};
        return string {func_name.substr(name_begin, name_end - name_begin)} +
               "::" + string {func_name.substr(value_begin, value_end - value_begin)};
    }

public:
    template <typename T>
        requires requires(OutStream& out, const T& obj) { out << obj; } && (!is_int<T>) &&
                 (!is_floating_point_v<T>) && (!requires(T t) { typename T::element_type; }) &&
                 (!is_array_v<T>)
    PrintStream& operator<<(const T& obj)
    {
        *out << obj;

        return *this;
    }

    PrintStream& operator<<(ostream& (*f)(ostream&))
    {
        *out << f;
        return *this;
    }

    template <typename T>
        requires ranges::input_range<const T> && (!convertible_to<const T, string_view>) &&
                 (!requires { std::tuple_size<T>::value; }) &&
                 (!requires { typename T::key_type; }) && (!requires { typename T::mapped_type; })
    PrintStream& operator<<(const T& list)
    {
        size_t list_size {};
        if constexpr (ranges::sized_range<T>) { list_size = ranges::size(list); }
        else { list_size = ranges::distance(list); }

        if (list_size > max_print)
        {
            *out << '[' << "<size = " << list_size << "> ";
            for (const auto& elem : list | views::take(max_print / 2))
            {
                *this << elem;
                *out << ',';
            }
            *out << "...";
            for (const auto& elem : list | views::drop(list_size - (max_print / 2)))
            {
                *out << ',';
                *this << elem;
            }
            *out << ']';

            return *this;
        }

        *out << '[';
        if (list_size > size_threshold) { *out << "(size = " << list_size << ") "; }
        for (const auto& elem : list | views::take(1)) { *this << elem; }
        for (const auto& elem : list | views::drop(1))
        {
            *out << ',';
            *this << elem;
        }
        *out << ']';

        return *this;
    }

    template <typename T>
        requires requires { std::tuple_size<T>::value; }
    PrintStream& operator<<(const T& tuple) // pair、array、tuple
    {
        if constexpr (tuple_size_v<T> == 0)
        {
            *out << "()";
            return *this;
        }
        else
        {
            *out << '(';
            apply(
                [this](const auto& arg, const auto&... args)
                {
                    *this << arg;
                    (..., (*out << ' ', *this << args));
                },
                tuple);
            *out << ')';

            return *this;
        }
    }

    template <typename T>
        requires requires { typename T::key_type; } && (!requires { typename T::mapped_type; })
    PrintStream& operator<<(const T& set)
    {
        size_t set_size {ranges::size(set)};

        if (set_size > max_print)
        {
            *out << '{' << "<size = " << set_size << "> ";
            for (const auto& key : set | views::take(max_print / 2))
            {
                *this << key;
                *out << ',';
            }
            *out << "...";
            for (const auto& key : set | views::drop(set_size - (max_print / 2)))
            {
                *out << ',';
                *this << key;
            }
            *out << '}';

            return *this;
        }

        *out << '{';
        if (set_size > size_threshold) { *out << "<size = " << set_size << "> "; }
        for (const auto& key : set | views::take(1)) { *this << key; }
        for (const auto& key : set | views::drop(1))
        {
            *out << ',';
            *this << key;
        }
        *out << '}';

        return *this;
    }

    template <typename T>
        requires requires { typename T::key_type; } && requires { typename T::mapped_type; }
    PrintStream& operator<<(const T& map)
    {
        size_t map_size {ranges::size(map)};

        if (map_size > max_print)
        {
            *out << '{' << "<size = " << map_size << "> ";
            for (const auto& [key, value] : map | views::take(max_print / 2))
            {
                *this << key;
                *out << ':';
                *this << value;
                *out << ',';
            }
            *out << "...";
            for (const auto& [key, value] : map | views::drop(map_size - (max_print / 2)))
            {
                *out << ',';
                *this << key;
                *out << ':';
                *this << value;
            }
            *out << '}';

            return *this;
        }

        *out << '{';
        if (map_size > size_threshold) { *out << "<size = " << map_size << "> "; }
        for (const auto& [key, value] : map | views::take(1))
        {
            *this << key;
            *out << ':';
            *this << value;
        }
        for (const auto& [key, value] : map | views::drop(1))
        {
            *out << ',';
            *this << key;
            *out << ':';
            *this << value;
        }
        *out << '}';

        return *this;
    }

    template <typename T>
        requires requires { typename T::container_type; }
    PrintStream& operator<<(const T& adapter)
    {
        struct AdapterVisitor : T
        {
            static const auto& get(const T& adapter) { return adapter.*(&AdapterVisitor::c); }
        };

        *this << AdapterVisitor::get(adapter);

        return *this;
    }

    template <typename T>
        requires is_int<T>
    PrintStream& operator<<(const T& number)
    {
        array<char, 1024> buf {};
        to_chars_result res = to_chars(begin(buf), end(buf), number, base);

        if (auto [end_ptr, err] = res; err == errc {}) [[likely]]
        {
            if (show_base)
            {
                if (base == 2) { out->write("0b", 2); }
                else if (base == 8) { out->write("0o", 2); }
                else if (base == 16) { out->write("0x", 2); }
            }
            out->write(data(buf), end_ptr - begin(buf));

            return *this;
        }

        cerr << "std::to_chars() failed";
        *out << number;

        return *this;
    }


    template <typename T>
        requires is_floating_point_v<T>
    PrintStream& operator<<(const T& number)
    {
        array<char, 1024> buf {};
        to_chars_result res = to_chars(begin(buf), end(buf), number, float_fmt, precision);

        if (auto [end_ptr, err] = res; err == errc {}) [[likely]]
        {
            out->write(data(buf), end_ptr - begin(buf));
            return *this;
        }

        cerr << "std::to_chars() failed";
        *out << number;

        return *this;
    }

    PrintStream& operator<<(const byte& b)
    {
        *out << static_cast<char>(b) << '(' << static_cast<unsigned int>(b) << ')';
        return *this;
    }

    template <typename T>
        requires requires(T t) { typename T::element_type; }
    PrintStream& operator<<(const T& smart_pointer)
    {
        if constexpr (requires { T {}.expired(); })
        {
            *out << "weak_ptr with use_count = " << smart_pointer.use_count();
        }
        else if constexpr (requires { T {}.use_count(); })
        {
            *out << "shared_ptr with use_count = " << smart_pointer.use_count()
                 << ", addr = " << smart_pointer;
        }
        else if constexpr (requires { T {}.release(); })
        {
            *out << "unique_ptr with addr = " << smart_pointer;
        }
        else { *out << "smart pointer"; }

        return *this;
    }

    template <typename T>
        requires input_or_output_iterator<T> && (!is_pointer_v<T>)
    PrintStream& operator<<(const T&)
    {
        if constexpr (contiguous_iterator<T>)
        {
            *out << "contiguous_iterator";
            return *this;
        }
        if constexpr (random_access_iterator<T>)
        {
            *out << "random_access_iterator";
            return *this;
        }
        if constexpr (bidirectional_iterator<T>)
        {
            *out << "bidirectional_iterator";
            return *this;
        }
        if constexpr (forward_iterator<T>)
        {
            *out << "forward_iterator";
            return *this;
        }
        if constexpr (input_iterator<T>)
        {
            *out << "input_iterator";
            return *this;
        }
        if constexpr (same_as<typename iterator_traits<T>::iterator_category, output_iterator_tag>)
        {
            *out << "output_iterator";
            return *this;
        }

        *out << "unknown_iterator_type";
        return *this;
    }

    template <typename T>
        requires is_enum_v<T>
    PrintStream& generic_to_string(const T& enumer)
    {
        *out << type_name<T>() << '(' << static_cast<underlying_type_t<T>>(enumer) << ')';
        return *this;
    }

    template <typename T>
        requires is_base_of_v<istream, T>
    PrintStream& operator<<(T& in)
    {
        *out << "input stream with state = ";

        auto state {in.rdstate()};

        if (state == ios::goodbit) { *out << "good, "; }
        else
        {
            if ((state & ios::badbit) != 0) { *out << "bad "; }
            if ((state & ios::failbit) != 0) { *out << "fail "; }
            if ((state & ios::eofbit) != 0) { *out << "eof "; }
            *out << '\n';

            return *this;
        }

        auto beg_pos {in.tellg()};
        if (beg_pos == -1)
        {
            *out << "This input stream is not seekable.";
            return *this;
        }
        in.seekg(0, istream::end);
        auto stream_size {in.tellg() - beg_pos};
        in.seekg(beg_pos);

        *out << "size = " << stream_size << ", line = "
             << count_if(istreambuf_iterator<char> {in}, {}, [](char c) { return c == '\n'; })
             << '\n';
        in.seekg(beg_pos);

        if (stream_size > 0)
        {
            *out << in.rdbuf();
            in.seekg(beg_pos);
        }

        return *this;
    }

    template <typename T>
    PrintStream& operator<<(const optional<T>& opt)
    {
        if (opt.has_value())
        {
            *out << "optional with value = ";
            return *this << opt.value();
        }

        *out << "nullopt";
        return *this;
    }

    PrintStream& operator<<(const nullopt_t& opt)
    {
        *out << "nullopt";
        return *this;
    }

    template <typename... Types>
    PrintStream& operator<<(const variant<Types...>& var)
    {
        if (var.valueless_by_exception()) [[unlikely]]
        {
            *out << "variant: valueless by exception";
            return *this;
        }

        *out << "variant(" << var.index() << ", ";

        visit(
            [this]<typename T>(const T& arg)
            {
                *out << type_name<T>() << ") = ";
                *this << arg;
            },
            var);

        return *this;
    }

    PrintStream& operator<<(const type_info& type)
    {
        if constexpr (need_demangle)
        {
            unique_ptr<char, decltype(&free)> demangled {
                abi::__cxa_demangle(type.name(), nullptr, nullptr, nullptr),
                free};

            if (demangled) { *out << demangled.get(); }
            else { *out << type.name(); }
        }
        else { *out << type.name(); }

        return *this;
    }

    template <typename T>
        requires same_as<T, any>
    PrintStream& operator<<(const T& any)
    {
        *out << "any<";
        *this << any.type();
        *out << '>';

        return *this;
    }

    template <typename Rep, typename Period>
    PrintStream& operator<<(const chrono::duration<Rep, Period>& duration)
    {
        if (nano) { *out << chrono::duration_cast<chrono::nanoseconds>(duration).count() << "ns"; }
        else if (micro)
        {
            *out << chrono::duration_cast<chrono::microseconds>(duration).count() << "us";
        }
        else if (milli)
        {
            *out << chrono::duration_cast<chrono::milliseconds>(duration).count() << "ms";
        }
        else { *out << chrono::duration_cast<chrono::seconds>(duration).count() << "s"; }

        return *this;
    }

    PrintStream& operator<<(const chrono::system_clock::time_point point)
    {
        static const auto* zone {chrono::current_zone()};
        *out << chrono::zoned_time {zone, point};

        return *this;
    }

    template <typename T>
    PrintStream& operator<<(const T&)
    {
        *out << type_name<T>();

        return *this;
    }
};

inline PrintStream gout {cout};
