#ifndef __TYPEDATAEXTRACTOR_H__
#define __TYPEDATAEXTRACTOR_H__

#include "Extractor/Data/TypeData.h"

#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace Extractor {
    namespace TypeDataExtractor {
        bool IsAType(const std::string& line, std::smatch& outMatch);
        TypeData Extract(const std::smatch& match, const std::string& fileName, const std::string& ns, const std::vector<std::string>& knownDefines, std::istream& stream);
    } // namespace TypeDataExtractor
} // namespace Extractor
#endif // __TYPEDATAEXTRACTOR_H__