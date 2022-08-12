#ifndef __INCLUDEMAPGENERATOR_H__
#define __INCLUDEMAPGENERATOR_H__

#include "Extractor/Data/IncludeMap.h"

#include <string>
#include <vector>

namespace Extractor {
    struct FileData;

    IncludeMap GenerateIncludeMap(const std::vector<FileData>& files);

    bool HasCircularDependency(const IncludeMap& includeMap, const std::string& fileName);
    std::vector<std::string> GetCircularDependencyTrail(const IncludeMap& includeMap, const std::string& fileName);
} // namespace Extractor
#endif // __INCLUDEMAPGENERATOR_H__