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
        FileDataExtractor(std::string filePath, std::vector<std::string> userDefines, std::unordered_map<std::string, PreProcessorResult>& preProcessedFiles, ExtractorSettings settings)
            : m_FilePath(filePath)
            , m_UserDefines(userDefines)
            , m_PreProcessedFiles(&preProcessedFiles)
            , m_Settings(settings) {}
        FileDataExtractor(const FileDataExtractor&) = delete;
        FileDataExtractor& operator=(const FileDataExtractor&) = delete;

        FileData Execute() const;

    private:
        std::string m_FilePath;
        std::vector<std::string> m_UserDefines;
        std::unordered_map<std::string, PreProcessorResult>* m_PreProcessedFiles;
        ExtractorSettings m_Settings;
    };

} // namespace Extractor
#endif // __FILEDATAEXTRACTOR_H__