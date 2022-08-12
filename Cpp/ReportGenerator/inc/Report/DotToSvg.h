#pragma once

#include <string>

namespace Report {
    void ConvertDotsToSvg(const std::string& directory);
    void DeleteDotFiles(const std::string& directory);
}