#ifndef __BASEREPORT_H__
#define __BASEREPORT_H__

#include "Extractor/Data/FileData.h"
#include "IReport.h"

#include <vector>

namespace Report {
    class BaseReport : public IReport {
    public:
        BaseReport(std::vector<Extractor::FileData> fileData) : m_FileData(fileData) {}
        virtual void WriteReport(std::string fileName) const = 0;

    protected:
        std::vector<Extractor::FileData> m_FileData;
    };
} // namespace Report
#endif // __BASEREPORT_H__