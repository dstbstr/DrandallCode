#pragma once

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