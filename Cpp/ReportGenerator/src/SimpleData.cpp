#include "Report/Data/SimpleData.h"

#include "Extractor/Data/FileData.h"

namespace Report {
    SimpleData SimpleData::Create(const Extractor::FileData& data, std::string filePath, std::string fileName) {
        SimpleData result{};
        result.FilePath = filePath;
        result.FileName = fileName;
        result.DirectDependencies = std::to_string(data.IncludeFiles.size());
        result.TotalDependencies = std::to_string(data.TotalIncludeCount);
        result.LinesInFile = std::to_string(data.LineCount);
        result.TotalLines = std::to_string(data.TotalLineCount);
        result.IncludedBy = std::to_string(data.IncludedByCount);
        result.CouplingImpact = std::to_string(data.IncludedByCount * data.TotalLineCount);
        result.SizeImpact = std::to_string(data.IncludedByCount * data.TotalIncludeCount);

        return result;
    }
} // namespace Report