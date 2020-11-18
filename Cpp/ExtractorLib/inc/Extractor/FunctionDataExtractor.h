#ifndef __FUNCTIONDATAEXTRACTOR_H__
#define __FUNCTIONDATAEXTRACTOR_H__

#include "Extractor/Data/FunctionData.h"

#include <iostream>
#include <string>

namespace Extractor {
    namespace FunctionDataExtractor {
        bool IsAFunction(const std::string& line);
        FunctionData Extract(std::string line, std::istream& stream, const std::string& ns, const std::string& className, Visibility visibility);
    } // namespace FunctionDataExtractor
} // namespace Extractor
#endif // __FUNCTIONDATAEXTRACTOR_H__