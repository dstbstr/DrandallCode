#pragma once

#include "Report/BaseReport.h"

namespace Report {
    class ExcelReport : public BaseReport {
    public:
        using BaseReport::BaseReport;
        void WriteReport(std::string fileName) const final;
    };
} // namespace Report