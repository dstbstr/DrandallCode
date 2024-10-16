#pragma once
#include <string>
#include <unordered_map>

namespace Extractor {
    struct DefineData {
        std::unordered_map<std::string, std::string> Defines;
        std::unordered_map<std::string, std::string> DefineSource;
    };
} // namespace Extractor
