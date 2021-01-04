#include "IncludeReport.h"

#include "Extractor/Data/FileData.h"
#include "Instrumentation/Assert.h"
#include "Utilities/Format.h"
#include "Utilities/Require.h"

#include <algorithm>

constexpr char LineFormat[]{"%s,%u,%u,%u,%u,%u,%u,%u\n"};
constexpr char LineHeading[]{"File Name,Direct Dependenciess,Total Dependencies,Lines In File,Total Lines,Included By,Coupling Impact,Size Impact"};

namespace IncludeCounter {
    IncludeReport::ReportLine::ReportLine(Extractor::FileData data) : Data(data) {
        SizeImpact = data.IncludedByCount * data.TotalLineCount;
        CouplingImpact = data.IncludedByCount * data.TotalIncludeCount;
    }

    std::string IncludeReport::ReportLine::ToString() const {
        return StrUtil::Format(LineFormat, Data.FilePath, Data.IncludeFiles.size(), Data.TotalIncludeCount, Data.LineCount, Data.TotalLineCount, Data.IncludedByCount, CouplingImpact, SizeImpact);
    }

    IncludeReport::IncludeReport(const std::vector<Extractor::FileData>& results) {
        m_Lines.reserve(results.size());
        for(auto&& result: results) {
            m_Lines.push_back(ReportLine(result));
        }
    }

    void IncludeReport::PrintResultToStream(std::ostream& targetStream) const {
        targetStream << LineHeading << std::endl;
        for(auto&& line: m_Lines) {
            targetStream << line.ToString();
        }
    }
} // namespace IncludeCounter