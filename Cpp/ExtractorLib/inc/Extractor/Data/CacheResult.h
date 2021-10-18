#ifndef __CACHERESULT_H__
#define __CACHERESULT_H__

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Extractor {
    struct FileData;

    struct DefineCache {
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> FileDefines;
    };
    struct IncludeCache {
        std::unordered_map<std::string, std::unordered_set<std::string>> FileIncludes;
        std::unordered_map<std::string, std::unordered_set<std::string>> FileIncludedBy;
    };
    struct FileDataCache {
        std::unordered_map<std::string, Extractor::FileData> FileData;
    };

    struct CacheResult {
        std::filesystem::file_time_type CacheTime;
        DefineCache DefineCache;
        IncludeCache IncludeCache;
        FileDataCache FileDataCache;
    };
} // namespace Extractor
#endif // __CACHERESULT_H__