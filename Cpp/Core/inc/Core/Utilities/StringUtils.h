#pragma once

#include "Core/Platform/Types.h"
#include "Core/Utilities/LambdaUtils.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

template<typename T>
inline std::string ToString(T input) {
    static_assert(std::is_integral_v<T>, "ToString not specialized for Type");
    auto buf = std::unique_ptr<char[]>(new char[20]);
    for (auto i = 0; i < 20; i++) {
        buf[i] = 0;
    }
    std::to_chars(buf.get(), buf.get() + 20, input);
    return std::string(buf.get());
}

template<>
inline std::string ToString(const std::string& input) {
    return input;
}
template<>
inline std::string ToString(const std::string_view& input) {
    return std::string(input);
}
template<typename TElement>
inline std::string ToString(const std::vector<TElement>& input) {
    std::stringstream stream;
    for (const auto& i : input) {
        stream << ToString(i) << ", ";
    }
    auto str = stream.str();

    if (str.empty()) {
        return "";
    }

    return str.substr(0, str.size() - 2);
}

namespace StrUtil {
    template<typename Collection>
    std::string JoinVec(std::string&& delimiter, Collection&& input) {
        std::stringstream stream;
        bool first = true;
        for (auto it = input.begin(); it != input.end(); it++) {
            if (first) {
                first = false;
            }
            else {
                stream << delimiter;
            }
            stream << *it;
        }

        return stream.str();
    }

    template<typename Collection>
    std::string JoinVec(std::string&& delimiter, Collection const& input) {
        std::stringstream stream;
        bool first = true;
        for (auto it = input.begin(); it != input.end(); it++) {
            if (first) {
                first = false;
            }
            else {
                stream << delimiter;
            }
            stream << *it;
        }

        return stream.str();
    }

    template<typename Collection>
    std::string JoinVec(char&& delimiter, Collection const& input) {
        return JoinVec(std::string(1, delimiter), input);
    }

    template<typename Head, typename... Tail>
    std::string Join(const std::string& delimiter, Head&& head, Tail&&... args) {
        std::stringstream stream;

        stream << std::forward<Head>(head);

        LambdaUtils::ForEach([&](auto it) { stream << delimiter << it; }, args...);

        return stream.str();
    }

    template<typename Head, typename... Tail>
    std::string Join(char&& delimiter, Head&& head, Tail&&... args) {
        std::stringstream stream;

        stream << std::forward<Head>(head);

        LambdaUtils::ForEach([&](auto it) { stream << delimiter << it; }, args...);

        return stream.str();
    }

    std::vector<std::string_view> Split(std::string_view input, std::string_view delimiter);

    bool EqualsIgnoreCase(std::string_view lhs, std::string_view rhs);
    std::string ToUpper(const std::string& str);
    std::string ToLower(const std::string& str);

    static inline std::string TrimStart(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        return s;
    }

    static inline std::string TrimEnd(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        return s;
    }

    static inline std::string Trim(std::string s) {
        return TrimStart(TrimEnd(s));
    }

    bool StartsWith(std::string_view string, std::string_view prefix);
    bool EndsWith(std::string_view string, std::string_view suffix);
} // namespace StrUtil

