#ifndef __INCLUDEMAP_H__
#define __INCLUDEMAP_H__

#include "Platform/Types.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Extractor {
    struct IncludeMap {
        std::unordered_map<std::string, std::unordered_set<std::string>> Dependencies;
        std::unordered_map<std::string, std::unordered_set<std::string>> DependsOnMe;
        std::unordered_map<std::string, u64> LineCounts;
    };
} // namespace Extractor
#endif // __INCLUDEMAP_H__