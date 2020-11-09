#ifndef __FUNCTIONDATAEXTRACTOR_H__
#define __FUNCTIONDATAEXTRACTOR_H__

#include "IncludeCounter/Data/FunctionData.h"

#include <string>


namespace IncludeCounter {
    namespace Extractors {
        namespace FunctionDataExtractor {
            bool IsLineAFunction(const std::string& line);
            FunctionData Extract(const std::string& line);
        } // namespace FunctionDataExtractor
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __FUNCTIONDATAEXTRACTOR_H__