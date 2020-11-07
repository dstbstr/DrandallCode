#include "IncludeCountTask.h"

#include "Utilities/PathUtilities.h"

#include <fstream>
#include <regex>

namespace IncludeCounter {
    FileData IncludeCountTask::Execute() {
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
} // namespace IncludeCounter