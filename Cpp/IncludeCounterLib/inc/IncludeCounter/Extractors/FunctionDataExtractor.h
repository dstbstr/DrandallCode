#ifndef __FUNCTIONDATAEXTRACTOR_H__
#define __FUNCTIONDATAEXTRACTOR_H__

#include "IncludeCounter/Data/FunctionData.h"

#include <iostream>
#include <string>

namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsAFunction(const std::string& line);
            FunctionData Extract(std::string line, std::istream& stream, std::string className, Visibility visibility);
        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __FUNCTIONDATAEXTRACTOR_H__