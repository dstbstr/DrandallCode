#ifndef __INCLUDECOUNTTASK_H__
#define __INCLUDECOUNTTASK_H__

#include "IncludeCounter/FileData.h"
#include "Threading/IRunnable.h"

namespace IncludeCounter {
    class IncludeCountTask : public IRunnable<FileData> {
    public:
        explicit IncludeCountTask(std::string filePath) : m_FilePath(filePath) {}

        FileData Execute();

    private:
        std::string m_FilePath;
    };
} // namespace IncludeCounter
#endif // __INCLUDECOUNTTASK_H__