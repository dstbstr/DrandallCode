#pragma once

#include <string>

namespace Extractor {
    enum TypeKeyword { STRUCT, CLASS, UNION, ENUM };

    constexpr std::string ToString(TypeKeyword type) {
        switch(type) {
        case STRUCT: return "Struct";
        case CLASS: return "Class";
        case UNION: return "Union";
        case ENUM: return "Enum";
        default: return "Unknown";
        }
    }
} // namespace Extractor
