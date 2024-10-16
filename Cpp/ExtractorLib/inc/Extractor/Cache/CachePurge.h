#pragma once
#include <string>
#include <vector>


namespace Extractor {
    struct CacheResult;

    namespace Cache {
        void Purge(CacheResult& cache, std::vector<std::string> fileNames);
    }
} // namespace Extractor
