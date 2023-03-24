#pragma once

#include <vector>
#include <array>

namespace Constexpr {
    constexpr size_t Strlen(const char* str) {
        return std::char_traits<char>::length(str);
    }

    static_assert(Strlen("hello") == 5);
    static_assert(Strlen("a") == 1);
    static_assert(Strlen("") == 0);

    template<typename T>
    constexpr std::string ToString(T val) {
        bool negate = false;
        if constexpr (std::is_signed_v<T>) {
            if (val < 0) {
                negate = true;
                val = -val;
            }
        }

        std::string result;
        while (val > 10) {
            result.push_back('0' + (val % 10));
            val /= 10;
        }
        result.push_back('0' + (val % 10));
        if (negate) {
            result.push_back('-');
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    static_assert(Constexpr::ToString(1234) == "1234");
    static_assert(Constexpr::ToString(-1234) == "-1234");

    constexpr std::vector<std::string_view> Split(std::string_view input, std::string_view delimiter) {
        size_t last = 0;
        size_t next = 0;
        std::vector<std::string_view> result;

        while ((next = input.find(delimiter, last)) != std::string::npos) {
            if (next - last > 0) {
                result.push_back(input.substr(last, next - last));
            }
            last = next + delimiter.size();
        }

        auto lastEntry = input.substr(last);
        if (lastEntry.length() > 0) {
            result.push_back(lastEntry);
        }
        return result;
    }

    static_assert(Split("a b c", " ").size() == 3);
    static_assert(Split("abc", " ").size() == 1);

    template<typename T>
    constexpr bool ParseNumber(std::string_view input, T& result) {
        T place = 1;
        size_t pos = input[0] == '-' || input[0] == '+' ? 1 : 0;
        result = 0;
        for (size_t i = input.size() - 1; i != pos; i--) {
            if (input[i] < '0' || input[i] > '9') {
                return false;
            }
            result += (input[i] - '0') * place;
            place *= 10;
        }
        if (input[pos] < '0' || input[pos] > '9') return false;

        result += (input[pos] - '0') * place;
        if constexpr (std::is_signed_v<T>) {
            if (input[0] == '-') {
                result = -result;
            }
        }
        else {
            if (input[0] == '-') {
                return false;
            }
        }

        return true;
    }

    namespace ContexprTests {
        template<typename T>
        constexpr bool TestParseNumber(const std::string& str, T expected) {
            T actual;
            auto success = ParseNumber<T>(str, actual);
            return success && actual == expected;
        }

        static_assert(TestParseNumber<unsigned int>("1234", 1234));
        static_assert(TestParseNumber<int>("-1234", -1234));
        static_assert(TestParseNumber<size_t>("123454321", 123454321));
        static_assert(TestParseNumber<long long>("1234567654321", 1234567654321));

        static_assert(TestParseNumber("42", 42));
        static_assert(TestParseNumber("-42", -42));
        static_assert(TestParseNumber("0", unsigned int(0)));
        static_assert(!TestParseNumber("-1", int(1)));
        static_assert(!TestParseNumber("abc", 0));
    }

}