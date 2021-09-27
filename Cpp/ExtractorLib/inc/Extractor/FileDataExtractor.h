#ifndef __FILEDATAEXTRACTOR_H__
#define __FILEDATAEXTRACTOR_H__

#include "Extractor/Data/FileData.h"
#include "Extractor/Data/PreProcessorResult.h"
#include "Extractor/ExtractorSettings.h"
#include "Threading/IRunnable.h"

#include <unordered_map>

namespace Extractor {
    class FileDataExtractor : public IRunnable<FileData> {
    public:
        FileDataExtractor(std::string filePath, const std::unordered_map<std::string, std::string>& defines, ExtractorSettings settings) : m_FilePath(filePath), m_Defines(&defines), m_Settings(settings) {}
        FileDataExtractor(const FileDataExtractor&) = delete;
        FileDataExtractor& operator=(const FileDataExtractor&) = delete;

        FileData Execute() const;

    private:
        std::string m_FilePath;
        const std::unordered_map<std::string, std::string>* m_Defines;
        ExtractorSettings m_Settings;
    };

} // namespace Extractor
#endif // __FILEDATAEXTRACTOR_H__