#ifndef __INCLUDEREPORT_H__
#define __INCLUDEREPORT_H__

#include "Platform/Types.h"

#include <ostream>
#include <vector>

namespace IncludeCounter {
    struct FileData;

    class IncludeReport {
    public:
        enum SortOrder : u8 { FILE_NAME, DIRECT_DEPENDENCIES, TOTAL_DEPENDENCIES, BLAST_RADIUS, _COUNT };
        static SortOrder GetSortOrder(u8 index);
        static std::vector<std::string> GetSortOrderStrings();

        IncludeReport(const std::vector<FileData>& results) : m_Results(results) {}
        void PrintResultToStream(std::ostream& targetStream, SortOrder sortBy = SortOrder::DIRECT_DEPENDENCIES, bool descending = false) const;

    private:
        const std::vector<FileData>& m_Results;
    };
} // namespace IncludeCounter
#endif // __INCLUDEREPORT_H__