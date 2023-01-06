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
    constexpr bool ParseNumber(std::string_view input, T& result, typename std::enable_if_t<std::is_signed_v<T>, bool> = true) {
        u32 place = 1;
        u32 pos = input[0] == '-' ? 1 : 0;

        result = 0;
        for (size_t i = input.size() - 1; i != pos; i--) {
            if (input[i] < '0' || input[i] > '9') {
                return false;
            }
            result += (input[i] - '0') * place;
            place *= 10;
        }
        result += (input[pos] - '0') * place;
#pragma warning(push)
#pragma warning(disable:4146)
        if (input[0] == '-') {
            result = -result;
        }
#pragma warning(pop)
        return true;
    }

    template<typename T>
    constexpr bool ParseNumber(std::string_view input, T& result, typename std::enable_if_t<!std::is_signed_v<T>, bool> = true) {
        T place = 1;
        if (input[0] == '-') {
            return false;
        }

        result = 0;
        for (size_t i = input.size() - 1; i != 0; i--) {
            if (input[i] < '0' || input[i] > '9') {
                return false;
            }
            result += (input[i] - '0') * place;
            place *= 10;
        }
        result += (input[0] - '0') * place;

        return true;
    }

    namespace ContexprTests {
        template<typename T>
        constexpr bool TestParseNumber(const std::string& str, T expected) {
            T actual;
            auto success = ParseNumber<T>(str, actual);
            return success && actual == expected;
        }

        static_assert(TestParseNumber<u32>("1234", 1234));
        static_assert(TestParseNumber<s32>("-1234", -1234));
        static_assert(TestParseNumber<u64>("123454321", 123454321));

        static_assert(TestParseNumber("42", 42));
        static_assert(TestParseNumber("-42", -42));
        static_assert(TestParseNumber("0", u32(0)));
        static_assert(!TestParseNumber("-1", s32(1)));
        static_assert(!TestParseNumber("abc", 0));
    }

}