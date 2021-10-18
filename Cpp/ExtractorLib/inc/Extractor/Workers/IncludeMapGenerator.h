#ifndef __INCLUDEMAPGENERATOR_H__
#define __INCLUDEMAPGENERATOR_H__

#include "Extractor/Data/IncludeMap.h"

#include <string>
#include <vector>

namespace Extractor {
    struct FileData;

    IncludeMap GenerateIncludeMap(const std::vector<FileData>& files);
} // namespace Extractor
#endif // __INCLUDEMAPGENERATOR_H__