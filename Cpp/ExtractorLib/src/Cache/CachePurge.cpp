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
        if(cache.Defines.erase(fileName) == 0) {
            return;
        }

        for(const auto& dep: cache.IncludedByMap.at(fileName)) {
            PurgeRecurse(cache, dep);
        }

        cache.IncludedByMap.erase(fileName);
        cache.FileData.erase(fileName);
    }
} // namespace

namespace Extractor {
    namespace Cache {
        void Purge(CacheResult& cache, std::vector<std::string> fileNames) {
            if(cache.Defines.size() == 0) {
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