#include "Core/Utilities/StringUtils.h"

#include "Core/Utilities/Require.h"

namespace StrUtil {

    bool EqualsIgnoreCase(std::string_view lhs, std::string_view rhs) {
        static auto comparer = [](const char& c1, const char& c2) {
            return c1 == c2 || std::toupper(c1) == std::toupper(c2);
        };

        return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin(), comparer);
    }

    std::string ToUpper(const std::string& str) {
        std::string result = str;
        std::transform(str.cbegin(), str.cend(), result.begin(), [](const char& letter) { return (char)std::toupper(letter); });
        return result;
    }

    std::string ToLower(const std::string& str) {
        std::string result = str;
        std::transform(str.cbegin(), str.cend(), result.begin(), [](const char& letter) { return (char)std::tolower(letter); });
        return result;
    }

    std::vector<std::string_view> Split(std::string_view input, std::string_view delimiter) {
        size_t last = 0;
        size_t next = 0;
        std::vector<std::string_view> result;

        while((next = input.find(delimiter, last)) != std::string::npos) {
            if(next - last > 0) {
                result.push_back(input.substr(last, next - last));
            }
            last = next + delimiter.size();
        }

        auto lastEntry = input.substr(last);
        if(lastEntry.length() > 0) {
            result.push_back(lastEntry);
        }
        return result;
    }

    bool StartsWith(std::string_view string, std::string_view prefix) {
        return string.rfind(prefix, 0) == 0;
    }

    bool EndsWith(std::string_view string, std::string_view suffix) {
        return string.rfind(suffix) == string.length() - suffix.length() && string.length() >= suffix.length();
    }
} // namespace StrUtil