#ifndef __FUNCTIONREPORT_H__
#define __FUNCTIONREPORT_H__

#include "Extractor/Data/FileData.h"
#include "Extractor/Data/FunctionData.h"
#include "IReport.h"
#include "Platform/Types.h"

#include <vector>

namespace TypeCounter {
    class FunctionReport : public IReport {
    public:
        FunctionReport(const std::vector<Extractor::FileData>& results);
        void PrintResultToStream(std::ostream& targetStream) const final;

    private:
        struct ReportLine {
            ReportLine(Extractor::FunctionData data);
            std::string ToString() const;
            Extractor::FunctionData Data;
        };

        std::vector<ReportLine> m_Lines;
    };
}; // namespace TypeCounter

#endif // __FUNCTIONREPORT_H__