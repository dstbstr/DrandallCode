#ifndef __PATHUTILS_H__
#define __PATHUTILS_H__

#include <regex>
#include <string>

namespace PathUtils {
    std::string GetFileName(const std::string& pathOrFile);
    bool PathContainsWildcard(const std::string& path);
    std::string GetWildcardPathParent(const std::string& path);
    std::regex PathWithWildcardToRegex(const std::string& path);
} // namespace PathUtils
#endif // __PATHUTILS_H__