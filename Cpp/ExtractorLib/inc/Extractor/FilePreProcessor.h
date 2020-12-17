#ifndef __FILEPREPROCESSOR_H__
#define __FILEPREPROCESSOR_H__

#include "Extractor/Data/PreProcessorResult.h"
#include "Threading/IRunnable.h"

#include <string>

namespace Extractor {
    class FilePreProcessor : public IRunnable<PreProcessorResult> {
    public:
        FilePreProcessor(std::string filePath) : m_FilePath(filePath) {}
        FilePreProcessor(const FilePreProcessor&) = delete;
        FilePreProcessor& operator=(const FilePreProcessor&) = delete;

        PreProcessorResult Execute() const;

    private:
        std::string m_FilePath;
    };
} // namespace Extractor

#endif // __FILEPREPROCESSOR_H__