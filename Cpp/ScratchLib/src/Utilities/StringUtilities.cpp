#include "Utilities/StringUtilities.h"

namespace StrUtil {

    bool EqualsIgnoreCase(const std::string& lhs, const std::string& rhs) {
        static auto comparer = [](const char& c1, const char& c2) { return c1 == c2 || std::toupper(c1) == std::toupper(c2); };

        return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin(), comparer);
    }

    std::vector<std::string> Split(std::string input, std::string delimiter) {
        size_t last = 0;
        size_t next = 0;
        std::vector<std::string> result;

        while ((next = input.find(delimiter, last)) != std::string::npos) {
            if (next - last > delimiter.size()) {
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
} // namespace StrUtil