#ifndef __SPECIALFUNCTIONTYPE_H__
#define __SPECIALFUNCTIONTYPE_H__

#include <string>
namespace Extractor {
    enum SpecialFunctionType { CONSTRUCTOR, DESTRUCTOR };
    static inline std::string ToString(SpecialFunctionType type) {
        switch(type) {
        case CONSTRUCTOR: return "Constructor";
        case DESTRUCTOR: return "Destructor";
        default: return "Unknown";
        }
    }

} // namespace Extractor
#endif // __SPECIALFUNCTIONTYPE_H__