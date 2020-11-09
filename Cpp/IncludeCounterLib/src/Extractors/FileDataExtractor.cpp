#include "IncludeCounter/Extractors/FileDataExtractor.h"
#include "IncludeCounter/Extractors/TypeDataExtractor.h"
#include "IncludeCounter/Extractors/FunctionDataExtractor.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/StringUtilities.h"

#include <fstream>
#include <regex>

namespace IncludeCounter {
    namespace Extractors {
        static std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
        static std::regex SingleLineCommentRegex("^//");

        FileData FileDataExtractor::Execute() {
            FileData result;
            result.FilePath = m_FilePath;
            result.FileName = PathUtils::GetFileName(m_FilePath);

            std::ifstream stream(m_FilePath, std::ifstream::in);
            if(!stream.is_open()) {
                return result;
            }

            std::string line;
            std::smatch match;

            while(std::getline(stream, line)) {
                auto trimmed = StrUtil::Trim(line);
                if(trimmed == "" || std::regex_match(trimmed, SingleLineCommentRegex)) {
                    continue;
                }
                if(std::regex_search(trimmed, match, IncludeRegex)) {
                    result.IncludeFiles.insert(match[1]);
                } else if(FunctionDataExtractor::IsLineAFunction(trimmed)) {
                    result.FreeFunctions.push_back(FunctionDataExtractor::Extract(trimmed));
                } else if(TypeDataExtractor::IsAType(trimmed)) {
                    result.Types.push_back(TypeDataExtractor::Extract(stream));
                }
            }

            return result;
        }
    } // namespace Executors
} // namespace IncludeCounter