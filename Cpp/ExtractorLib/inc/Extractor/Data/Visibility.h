#ifndef __VISIBILITY_H__
#define __VISIBILITY_H__

#include <string>

namespace Extractor {
    enum Visibility { PRIVATE, PROTECTED, PUBLIC };
    static inline std::string ToString(Visibility visibility) {
        switch(visibility) {
        case PRIVATE: return "Private";
        case PROTECTED: return "Protected";
        case PUBLIC: return "Public";
        default: return "Unknown";
        }
    }
} // namespace Extractor

#endif // __VISIBILITY_H__