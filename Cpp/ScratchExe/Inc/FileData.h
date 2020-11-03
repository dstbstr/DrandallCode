#ifndef __FILEDATA_H__
#define __FILEDATA_H__

#include "Platform/Types.h"

#include <string>
#include <unordered_set>

struct FileData {
public:
    std::string FileName;
    std::string FilePath;
    std::unordered_set<std::string> IncludeFiles;
    u64 TotalIncludeCount;
};

#endif // __FILEDATA_H__