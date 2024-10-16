#pragma once

#include <string>

namespace Extractor {
    enum SpecialFunctionType { CONSTRUCTOR, DESTRUCTOR };
    constexpr std::string ToString(SpecialFunctionType type) {
        switch(type) {
        case CONSTRUCTOR: return "Constructor";
        case DESTRUCTOR: return "Destructor";
        default: return "Unknown";
        }
    }

} // namespace Extractor