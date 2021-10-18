#ifndef __CACHERESULT_H__
#define __CACHERESULT_H__

#include "Extractor/Data/FileData.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Extractor {
    struct CacheResult {
        std::filesystem::file_time_type CacheTime;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> Defines;
        std::unordered_map<std::string, Extractor::FileData> FileData;
        std::unordered_map<std::string, std::unordered_set<std::string>> IncludedByMap;
    };
} // namespace Extractor
#endif // __CACHERESULT_H__