#ifndef __INCLUDEMAPGENERATOR_H__
#define __INCLUDEMAPGENERATOR_H__

#include "IncludeCounter/FileData.h"

#include <vector>

namespace IncludeCounter {
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
} // namespace IncludeCounterLib
#endif // __INCLUDEMAPGENERATOR_H__