#ifndef __TYPEKEYWORD_H__
#define __TYPEKEYWORD_H__

#include <string>

namespace Extractor {
    enum TypeKeyword { STRUCT, CLASS, UNION, ENUM };

    static inline std::string ToString(TypeKeyword type) {
        switch(type) {
        case STRUCT: return "Struct";
        case CLASS: return "Class";
        case UNION: return "Union";
        case ENUM: return "Enum";
        default: return "Unknown";
        }
    }
} // namespace Extractor
#endif // __TYPEKEYWORD_H__