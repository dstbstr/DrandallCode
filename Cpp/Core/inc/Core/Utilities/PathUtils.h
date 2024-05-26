#pragma once

#include <regex>
#include <string>

namespace PathUtils {
    std::string GetFileName(const std::string& pathOrFile);
    std::string GetFileDirectory(const std::string& pathToFile);
    bool PathContainsWildcard(const std::string& path);
    std::string GetWildcardPathParent(const std::string& path);
    std::regex PathWithWildcardToRegex(const std::string& path);
} // namespace PathUtils