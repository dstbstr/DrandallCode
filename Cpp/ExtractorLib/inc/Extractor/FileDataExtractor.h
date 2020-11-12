#ifndef __FILEDATAEXTRACTOR_H__
#define __FILEDATAEXTRACTOR_H__

#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"
#include "Threading/IRunnable.h"

namespace Extractor {
    class FileDataExtractor : public IRunnable<FileData> {
    public:
        explicit FileDataExtractor(std::string filePath, ExtractorSettings settings) : m_FilePath(filePath), m_Settings(settings) {}
        FileDataExtractor(const FileDataExtractor&) = delete;
        FileDataExtractor& operator=(const FileDataExtractor&) = delete;

        FileData Execute();

    private:
        std::string m_FilePath;
        ExtractorSettings m_Settings;
    };

} // namespace Extractor
#endif // __FILEDATAEXTRACTOR_H__