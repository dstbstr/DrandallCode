#include "IncludeCounter/Extractors/FileDataExtractor.h"

#include "IncludeCounter/Data/Visibility.h"
#include "IncludeCounter/Extractors/CommentExtractor.h"
#include "IncludeCounter/Extractors/FunctionDataExtractor.h"
#include "IncludeCounter/Extractors/TypeDataExtractor.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/StringUtilities.h"

#include <fstream>
#include <regex>

namespace {
    static std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
    static std::regex VisibilityRegex("^((private)|(public)|(protected)):");

} // namespace

namespace IncludeCounter {
    namespace Extractors {

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
            bool isHeader = m_FilePath[m_FilePath.length() - 1] == 'h';
            bool isInBlockComment = false;

            u64 nonBlankLines = 0;
            while(std::getline(stream, line)) {
                CommentExtractor::StripComments(line, isInBlockComment);
                auto trimmed = StrUtil::Trim(line);
                if(trimmed == "") { // do not check isInBlockComment here.  Line may end with a block comment, but start with code
                    continue;
                }
                nonBlankLines++;
                if(std::regex_search(trimmed, match, IncludeRegex)) {
                    result.IncludeFiles.insert(match[1]);
                }
                if(!isHeader) {
                    continue;
                }

                if(FunctionDataExtractor::IsLineAFunction(trimmed)) {
                    result.FreeFunctions.push_back(FunctionDataExtractor::Extract(stream));
                } else if(TypeDataExtractor::IsAType(trimmed)) {
                    result.Types.push_back(TypeDataExtractor::Extract(stream));
                }
            }

            result.LineCount = nonBlankLines;
            return result;
        }
    } // namespace Extractors
} // namespace IncludeCounter