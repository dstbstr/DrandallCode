#ifndef __FILEPREPROCESSOR_H__
#define __FILEPREPROCESSOR_H__

#include "Extractor/Data/PreProcessorResult.h"
#include "Threading/IRunnable.h"

#include <iostream>
#include <string>
#include <vector>

namespace Extractor {
    class FilePreProcessor : public IRunnable<PreProcessorResult> {
    public:
        FilePreProcessor(std::string filePath, const std::vector<std::string>& userDefines, std::shared_ptr<std::istream> stream = nullptr) : m_FilePath(filePath), m_UserDefines(userDefines), m_Stream(stream) {}
        FilePreProcessor(const FilePreProcessor&) = delete;
        FilePreProcessor& operator=(const FilePreProcessor&) = delete;

        PreProcessorResult Execute() const;
        void Reprocess(PreProcessorResult& existingResult, std::vector<std::string> knownDefines) const;

    private:
        std::string m_FilePath;
        std::vector<std::string> m_UserDefines;
        mutable std::shared_ptr<std::istream> m_Stream;
    };
} // namespace Extractor

#endif // __FILEPREPROCESSOR_H__