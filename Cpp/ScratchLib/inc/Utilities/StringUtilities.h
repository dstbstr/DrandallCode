#include <algorithm>
#include <cctype>
#include <locale>

namespace StrUtil
{
    // trim from start
    static inline std::string trimStart(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }));
        return s;
    }

    // trim from end
    static inline std::string trimEnd(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
        return s;
    }

    // trim both ends
    static inline std::string trim(std::string s)
    {
        std::string result = trimStart(s);
        return trimEnd(result);
    }
} // namespace StrUtil
