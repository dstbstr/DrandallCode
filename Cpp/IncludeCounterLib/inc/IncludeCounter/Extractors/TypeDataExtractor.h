#ifndef __TYPEDATAEXTRACTOR_H__
#define __TYPEDATAEXTRACTOR_H__

#include "IncludeCounter/Data/TypeData.h"

#include <iostream>
#include <string>

namespace IncludeCounter {
    namespace Extractors {
        namespace TypeDataExtractor {
            bool IsAType(const std::string& line);
            TypeData Extract(const std::string& initialLine, const std::string& fileName, std::istream& stream);
        } // namespace TypeDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __TYPEDATAEXTRACTOR_H__