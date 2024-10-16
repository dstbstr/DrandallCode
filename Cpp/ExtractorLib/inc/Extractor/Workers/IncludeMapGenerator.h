#pragma once

#include "Extractor/Data/IncludeMap.h"

#include <string>
#include <vector>

namespace Extractor {
    struct FileData;

    IncludeMap GenerateIncludeMap(const std::vector<FileData>& files);

    bool HasCircularDependency(const IncludeMap& includeMap, const std::string& fileName);
    std::vector<std::string> GetCircularDependencyTrail(const IncludeMap& includeMap, const std::string& fileName);
} // namespace Extractor