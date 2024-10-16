#pragma once

#include <string>

namespace Extractor {
    namespace DefineExtractor {
        bool CanExtract(const std::string& line);
        std::pair<std::string, std::string> Extract(const std::string& line);
    } // namespace DefineExtractor
} // namespace Extractor
