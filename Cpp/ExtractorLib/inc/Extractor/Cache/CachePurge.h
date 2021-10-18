#ifndef __CACHEPURGE_H__
#define __CACHEPURGE_H__

#include <string>
#include <vector>


namespace Extractor {
    struct CacheResult;

    namespace Cache {
        void Purge(CacheResult& cache, std::vector<std::string> fileNames);
    }
} // namespace Extractor
#endif // __CACHEPURGE_H__