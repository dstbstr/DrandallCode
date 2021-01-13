#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <iostream>

namespace FileUtils {
    bool Exists(const std::string_view filePath);
    std::unique_ptr<std::istream> OpenForRead(const std::string_view filePath);
} // namespace FileUtils
#endif // __FILEUTILS_H__