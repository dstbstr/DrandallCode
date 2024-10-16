#pragma once

#include <string>

namespace Extractor {
    enum Visibility { PRIVATE, PROTECTED, PUBLIC };
    constexpr std::string ToString(Visibility visibility) {
        switch(visibility) {
        case PRIVATE: return "Private";
        case PROTECTED: return "Protected";
        case PUBLIC: return "Public";
        default: return "Unknown";
        }
    }
} // namespace Extractor
