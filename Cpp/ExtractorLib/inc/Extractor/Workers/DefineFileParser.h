#pragma once

#include <string>
#include <unordered_map>

namespace Extractor {
    namespace DefineFileParser {
        std::unordered_map<std::string, std::string> Parse(std::istream& fileStream);
    }
} // namespace Extractor