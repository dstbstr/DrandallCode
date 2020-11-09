#ifndef __FILEDATA_H__
#define __FILEDATA_H__

#include "IncludeCounter/Data/FunctionData.h"
#include "IncludeCounter/Data/TypeData.h"
#include "Platform/Types.h"

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
        u64 LineCount{0};
        u64 TotalLineCount{0};
    };
} // namespace IncludeCounter
#endif // __FILEDATA_H__