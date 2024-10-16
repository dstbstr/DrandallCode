#pragma once

#include "Report/BaseReport.h"

namespace Report {
    class CsvReport : public BaseReport {
    public:
        using BaseReport::BaseReport;
        void WriteReport(std::string fileName) const final;
        void WriteReportToStream(std::ostream& targetStream) const;
    };
} // namespace Report