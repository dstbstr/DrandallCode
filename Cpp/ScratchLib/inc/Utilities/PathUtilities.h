#ifndef __PATHUTILITIES_H__
#define __PATHUTILITIES_H__

#include <string>

namespace PathUtils {
    static std::string GetFileName(std::string pathOrFile) {
        auto lastSeparator = pathOrFile.find_last_of("/\\");
        if(lastSeparator == std::string::npos) {
            return pathOrFile;
        }

        return pathOrFile.substr(lastSeparator + 1);
    }
} // namespace PathUtils
#endif // __PATHUTILITIES_H__