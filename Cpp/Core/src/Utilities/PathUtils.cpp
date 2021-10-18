#include "Utilities/PathUtils.h"

namespace PathUtils {
    std::string GetFileName(const std::string& pathOrFile) {
        auto lastSeparator = pathOrFile.find_last_of("/\\");
        if(lastSeparator == std::string::npos) {
            return pathOrFile;
        }

        return pathOrFile.substr(lastSeparator + 1);
    }

    std::string GetFileDirectory(const std::string& pathToFile) {
        auto lastSeparator = pathToFile.find_last_of("/\\");
        if(lastSeparator == pathToFile.npos) {
            return "";
        }

        return pathToFile.substr(0, lastSeparator + 1);
    }

    bool PathContainsWildcard(const std::string& path) {
        return path.find_last_of("*?") != path.npos;
    }

    std::string GetWildcardPathParent(const std::string& path) {
        auto lastWildcard = path.find_last_of("*?");
        if(lastWildcard == path.npos) {
            return path;
        }
        return path.substr(0, path.substr(0, lastWildcard).find_last_of("/\\"));
    }

    std::regex PathWithWildcardToRegex(const std::string& pathWithWildcard) {
        static std::regex backslashRegex("\\\\");
        static std::regex starRegex("\\*");
        static std::regex questionRegex("\\?");
        static std::regex dotRegex("\\.");
        std::string result = std::regex_replace(pathWithWildcard, backslashRegex, "\\\\"); // turn all single backslashes into escaped backslashes
        result = std::regex_replace(result, dotRegex, "\\."); // escape all dots to avoid them being interpreted as regex dots
        result = std::regex_replace(result, starRegex, R"([^\\\/]+)"); // Stars are any number of characters except path separators
        result = std::regex_replace(result, questionRegex, R"([^\\\/])"); // Question marks should be any 1 character that is not a path separator
        return std::regex(result);
    }
} // namespace PathUtils