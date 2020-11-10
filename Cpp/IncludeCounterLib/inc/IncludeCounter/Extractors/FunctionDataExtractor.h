#ifndef __FUNCTIONDATAEXTRACTOR_H__
#define __FUNCTIONDATAEXTRACTOR_H__

#include "IncludeCounter/Data/FunctionData.h"

#include <iostream>
#include <string>

namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsLineAFunction(const std::string& line);
            FunctionData Extract(std::istream& stream, Visibility visibility);
        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __FUNCTIONDATAEXTRACTOR_H__