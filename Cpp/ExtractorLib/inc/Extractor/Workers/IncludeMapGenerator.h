#ifndef __INCLUDEMAPGENERATOR_H__
#define __INCLUDEMAPGENERATOR_H__

#include "Extractor/Data/FileData.h"
#include "Platform/Types.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Extractor {
    struct IncludeMap {
        std::unordered_map<std::string, std::unordered_set<std::string>> Dependencies;
        std::unordered_map<std::string, std::unordered_set<std::string>> DependsOnMe;
        std::unordered_map<std::string, u64> LineCounts;
    };

    IncludeMap GenerateIncludeMap(const std::vector<FileData>& files);
} // namespace Extractor
#endif // __INCLUDEMAPGENERATOR_H__