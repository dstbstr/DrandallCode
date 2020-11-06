#ifndef __INCLUDECOUNTTASK_H__
#define __INCLUDECOUNTTASK_H__

#include "FileData.h"
#include "Threading/IRunnable.h"
#include "Utilities/PathUtilities.h"

#include <fstream>
#include <regex>

class IncludeCountTask : public IRunnable<FileData> {
public:
    explicit IncludeCountTask(std::string filePath) : m_FilePath(filePath) {}

    FileData Execute() {
        FileData result;
        result.FilePath = m_FilePath;
        result.FileName = PathUtils::GetFileName(m_FilePath);

        std::ifstream stream(m_FilePath, std::ifstream::in);
        if(!stream.is_open()) {
            return result;
        }

        std::string line;
        std::smatch match;
        std::regex includeRegex("^#include [\"<]([^\">]+)[\">]$");

        while(std::getline(stream, line)) {
            if(std::regex_search(line, match, includeRegex)) {
                result.IncludeFiles.insert(match[1]);
            }
        }

        return result;
    }

private:
    std::string m_FilePath;
};

#endif // __INCLUDECOUNTTASK_H__