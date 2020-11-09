#ifndef __FILEDATA_H__
#define __FILEDATA_H__

#include "Platform/Types.h"
#include "IncludeCounter/Data/FunctionData.h"
#include "IncludeCounter/Data/TypeData.h"

#include <string>
#include <unordered_set>

namespace IncludeCounter {
    struct FileData {
    public:
        std::string FileName;
        std::string FilePath;
        std::unordered_set<std::string> IncludeFiles;
        std::vector<FunctionData> FreeFunctions;
        std::vector<TypeData> Types;
        u64 TotalIncludeCount{0};
        u64 IncludedByCount{0};
    };
} // namespace IncludeCounterLib
#endif // __FILEDATA_H__