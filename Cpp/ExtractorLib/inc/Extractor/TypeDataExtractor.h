#ifndef __TYPEDATAEXTRACTOR_H__
#define __TYPEDATAEXTRACTOR_H__

#include "Extractor/Data/TypeData.h"

#include <iostream>
#include <string>

namespace Extractor {
    namespace TypeDataExtractor {
        bool IsAType(const std::string& line);
        TypeData Extract(const std::string& initialLine, const std::string& fileName, std::istream& stream);
    } // namespace TypeDataExtractor
} // namespace Extractor
#endif // __TYPEDATAEXTRACTOR_H__