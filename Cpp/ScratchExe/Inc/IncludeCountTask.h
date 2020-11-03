#ifndef __INCLUDECOUNTTASK_H__
#define __INCLUDECOUNTTASK_H__

#include "FileData.h"
#include "Threading/IRunnable.h"

#include <fstream>
#include <regex>

class IncludeCountTask : public IRunnable<FileData> {
public:
    explicit IncludeCountTask(std::string filePath) : m_FilePath(filePath) {}

    FileData Execute() {
        FileData result;
        result.FilePath = m_FilePath;
        result.FileName = ExtractFileName();

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

    std::string ExtractFileName() {
        auto lastSeparator = m_FilePath.find_last_of('/');
        if(lastSeparator < 0) {
            lastSeparator = m_FilePath.find_last_of('\\');
        }
        if(lastSeparator < 0) {
            return m_FilePath;
        }
        return m_FilePath.substr(lastSeparator + 1);
    }
};

#endif // __INCLUDECOUNTTASK_H__