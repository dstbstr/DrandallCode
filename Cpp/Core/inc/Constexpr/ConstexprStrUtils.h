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

    constexpr bool IsDigit(char c) {
        return c >= '0' && c <= '9';
    }

    static_assert(IsDigit('3'));
    static_assert(!IsDigit('A'));
    static_assert(IsDigit('0'));
    static_assert(IsDigit('9'));

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
        while (val >= 10) {
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

    static_assert(Constexpr::ToString(10) == "10");
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

    constexpr void ReplaceAll(std::string& original, const std::string& toFind, const std::string& replacement) {
        size_t index = 0;
        size_t length = toFind.length();

        while (true) {
            index = original.find(toFind, index);
            if (index == toFind.npos) break;
            original.replace(index, length, replacement);
        }
    }

    constexpr std::string ReplaceAllCopy(const std::string& original, const std::string& toFind, const std::string& replacement) {
        std::string result = original;
        ReplaceAll(result, toFind, replacement);
        return result;
    }

    constexpr void ReplaceOne(std::string& original, const std::string& toFind, const std::string& replacement) {
        size_t index = original.find(toFind, 0);
        if (index != original.npos) {
            original.replace(index, toFind.length(), replacement);
        }
    }

    constexpr std::string ReplaceOneCopy(const std::string& original, const std::string& toFind, const std::string& replacement) {
        std::string result = original;
        ReplaceOne(result, toFind, replacement);
        return result;
    }
    constexpr void RemoveAllOf(std::string& original, char toFind) {
        original.erase(std::remove(original.begin(), original.end(), toFind), original.end());
    }

    constexpr void RemoveAllOf(std::string& original, const std::string& toFind) {
        for (auto c : toFind) {
            RemoveAllOf(original, c);
        }
    }


    constexpr std::string RemoveAllOfCopy(const std::string& original, char toFind) {
        std::string result = original;
        RemoveAllOf(result, toFind);
        return result;
    }

    constexpr std::string RemoveAllOfCopy(const std::string& original, const std::string& toFind) {
        std::string result = original;
        RemoveAllOf(result, toFind);
        return result;
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

        static_assert(ReplaceAllCopy("abc", "a", "") == "bc");
        static_assert(ReplaceAllCopy("aaa", "a", "b") == "bbb");
        static_assert(ReplaceAllCopy("abbabbabb", "bb", "c") == "acacac");

        static_assert(RemoveAllOfCopy("abcabcabc", "cb") == "aaa");
        static_assert(RemoveAllOfCopy("abcabcabc", std::string("cb")) == "aaa");
        static_assert(RemoveAllOfCopy("abcabcabc", 'b') == "acacac");
    }
}