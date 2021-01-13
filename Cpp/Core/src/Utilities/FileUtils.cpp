#include "Utilities/FileUtils.h"

#include <filesystem>
#include <fstream>

namespace FileUtils {
    using namespace std::filesystem;

    bool Exists(const std::string_view filePath) {
        return exists(path(filePath));
    }

    std::unique_ptr<std::istream> OpenForRead(const std::string_view filePath) {
        return std::make_unique<std::ifstream>(filePath, std::ifstream::in);
    }
} // namespace FileUtils