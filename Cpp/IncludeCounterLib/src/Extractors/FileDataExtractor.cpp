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
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
} // namespace

// TODO: Add functionality for global variables?

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

                // it seems a lot easier to determine if the line is a type than if it's a function
                if(TypeDataExtractor::IsAType(trimmed)) {
                    auto type = TypeDataExtractor::Extract(trimmed, result.FileName, stream);
                    result.Types.push_back(type);
                } else if(FunctionDataExtractor::IsAFunction(trimmed)) {
                    result.FreeFunctions.push_back(FunctionDataExtractor::Extract(trimmed, stream, "", Visibility::PUBLIC));
                }
            }

            result.LineCount = nonBlankLines;
            return result;
        }
    } // namespace Extractors
} // namespace IncludeCounter