#ifndef __COMBINEDREPORT_H__
#define __COMBINEDREPORT_H__

#include "Extractor/Data/FileData.h"
#include "Extractor/Data/FunctionData.h"
#include "Extractor/Data/OperatorOverloadData.h"
#include "Extractor/Data/SpecialFunctionData.h"
#include "Extractor/Data/TypeData.h"
#include "IReport.h"

#include <vector>

namespace TypeCounter {
    class CombinedReport : public IReport {
    public:
        CombinedReport(const std::vector<Extractor::FileData>& results);
        void PrintResultToStream(std::ostream& targetStream) const final;

    private:
        void PrintTypeData(std::ostream& targetStream, Extractor::TypeData type) const;

        std::vector<Extractor::FileData> m_Files;
    };
} // namespace TypeCounter

#endif // __COMBINEDREPORT_H__