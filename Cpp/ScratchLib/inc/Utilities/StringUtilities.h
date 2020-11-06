#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include "Utilities/LambdaUtils.h"
#include "Platform/Types.h"

#include <algorithm>
#include <cctype>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace StrUtil {
    template<typename T>
    std::string JoinVec(std::string&& delimiter, std::vector<T>&& input) {
        std::stringstream stream;
        if(!input.empty()) {
            stream << input[0];
        }

        for(u32 i = 1; i < input.size(); i++) {
            stream << delimiter;
            stream << input[i];
        }

        return stream.str();
    }

    template<typename T>
    std::string JoinVec(std::string&& delimiter, std::vector<T> const& input) {
        std::stringstream stream;
        if(!input.empty()) {
            stream << input[0];
        }

        for(u32 i = 1; i < input.size(); i++) {
            stream << delimiter;
            stream << input[i];
        }

        return stream.str();
    }

    template<typename T>
    std::string JoinVec(char&& delimiter, std::vector<T> const& input) {
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
} // namespace StrUtil

#endif // __STRINGUTILITIES_H__