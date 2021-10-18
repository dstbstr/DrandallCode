#include "Extractor/Cache/CachePurge.h"

#include "Extractor/Data/CacheResult.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/TimeUtils.h"

#include <filesystem>

// remove
#include <iostream>

namespace {
    void PurgeRecurse(Extractor::CacheResult& cache, std::string fileName) {
        if(cache.DefineCache.FileDefines.erase(fileName) == 0) {
            return;
        }

        for(const auto& dep: cache.IncludeCache.FileIncludedBy.at(fileName)) {
            PurgeRecurse(cache, dep);
        }

        cache.IncludeCache.FileIncludedBy.erase(fileName);
        cache.IncludeCache.FileIncludes.erase(fileName);
    }
} // namespace

namespace Extractor {
    namespace Cache {
        void Purge(CacheResult& cache, std::vector<std::string> fileNames) {
            if(cache.DefineCache.FileDefines.size() == 0) {
                return;
            }

            for(const auto& fileName: fileNames) {
                if(std::filesystem::last_write_time(fileName) < cache.CacheTime) {
                    continue;
                }

                PurgeRecurse(cache, fileName);
            }
        }
    } // namespace Cache
} // namespace Extractor