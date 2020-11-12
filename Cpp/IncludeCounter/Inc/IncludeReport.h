#ifndef __INCLUDEREPORT_H__
#define __INCLUDEREPORT_H__

#include "Extractor/Data/FileData.h"
#include "Platform/Types.h"

#include <ostream>
#include <vector>

namespace IncludeCounter {

    class IncludeReport {
    public:
        IncludeReport(const std::vector<Extractor::FileData>& results);
        void PrintResultToStream(std::ostream& targetStream) const;

    private:
        struct ReportLine {
            ReportLine(Extractor::FileData data);
            std::string ToString() const;
            Extractor::FileData Data;
            u64 CouplingImpact;
            u64 SizeImpact;
        };

        std::vector<ReportLine> m_Lines;
    };
} // namespace IncludeCounter
#endif // __INCLUDEREPORT_H__