#ifndef __TYPEREPORT_H__
#define __TYPEREPORT_H__

#include "Extractor/Data/FileData.h"
#include "Extractor/Data/TypeData.h"
#include "IReport.h"

#include <vector>

namespace TypeCounter {
    class TypeReport : public IReport {
    public:
        TypeReport(const std::vector<Extractor::FileData>& results);
        void PrintResultToStream(std::ostream& targetStream) const final;

    private:
        struct ReportLine {
            ReportLine(Extractor::TypeData data);
            std::string ToString() const;
            Extractor::TypeData Data;
            u64 PublicMethodCount{0};
            u64 ProtectedMethodCount{0};
            u64 PrivateMethodCount{0};
        };

        std::vector<ReportLine> m_Lines;
    };
}; // namespace TypeCounter

#endif // __TYPEREPORT_H__