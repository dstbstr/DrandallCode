#ifndef __INCLUDEMAPGENERATOR_H__
#define __INCLUDEMAPGENERATOR_H__

#include "Extractor/Data/FileData.h"

#include <vector>

namespace Extractor {
    class IncludeMapGenerator {
    public:
        IncludeMapGenerator(std::vector<FileData>& files, bool failOnCircularDependencies = false)
            : m_Files(files)
            , m_FailOnCircularDependencies(failOnCircularDependencies) {}
        IncludeMapGenerator(std::vector<FileData>&&) = delete; // don't allow binding rvalue references
        IncludeMapGenerator(const IncludeMapGenerator&) = delete;
        const IncludeMapGenerator& operator=(IncludeMapGenerator&) = delete;

        void Generate();

    private:
        std::vector<FileData>& m_Files;
        bool m_FailOnCircularDependencies;
    };
} // namespace Extractor
#endif // __INCLUDEMAPGENERATOR_H__