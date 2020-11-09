#ifndef __TYPEDATAEXTRACTOR_H__
#define __TYPEDATAEXTRACTOR_H__

#include "IncludeCounter/Data/TypeData.h"

#include <string>
#include <fstream>

namespace IncludeCounter {
    namespace Extractors {
        namespace TypeDataExtractor {
            bool IsAType(const std::string& line);
            TypeData Extract(std::ifstream& stream);
        }
    } // namespace Extractors
} // namespace IncludeCounter
#endif // __TYPEDATAEXTRACTOR_H__