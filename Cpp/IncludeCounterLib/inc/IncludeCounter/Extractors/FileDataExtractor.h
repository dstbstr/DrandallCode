#ifndef __FILEDATAEXTRACTOR_H__
#define __FILEDATAEXTRACTOR_H__

#include "Threading/IRunnable.h"
#include "IncludeCounter/Data/FileData.h"

namespace IncludeCounter {
    namespace Extractors {
        class FileDataExtractor : IRunnable<FileData> {
        public:
            explicit FileDataExtractor(std::string filePath) : m_FilePath(filePath){}
            FileDataExtractor(const FileDataExtractor&) = delete;
            FileDataExtractor& operator=(const FileDataExtractor&) = delete;

            FileData Execute();
        private:
            std::string m_FilePath;
        };

    } // namespace Extractors
} // namespace IncludeCounter
#endif // __FILEDATAEXTRACTOR_H__