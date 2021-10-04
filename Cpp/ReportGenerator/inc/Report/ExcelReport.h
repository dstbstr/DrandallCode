#ifndef __EXCELREPORT_H__
#define __EXCELREPORT_H__

#include "Report/BaseReport.h"

namespace Report {
    class ExcelReport : public BaseReport {
    public:
        using BaseReport::BaseReport;
        void WriteReport(std::string fileName) const final;
    };
} // namespace Report

#endif // __EXCELREPORT_H__