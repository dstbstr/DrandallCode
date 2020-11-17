#include "Extractor/FileDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Extractor/Data/Visibility.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/TypeDataExtractor.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/StringUtilities.h"

#include <fstream>
#include <regex>


namespace {
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
    std::regex TemplateRegex("^template<[^>]*>$");
} // namespace

// TODO: Add functionality for global variables?
// todo: extract special functions? (constructors/destructors, operators)

namespace Extractor {

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
            if(m_Settings.CountIncludes && std::regex_search(trimmed, match, IncludeRegex)) {
                result.IncludeFiles.insert(match[1]);
            }
            if(!isHeader || (!m_Settings.ExtractTypes && !m_Settings.ExtractFunctions)) {
                continue;
            }

            // if template declaration is above type/function, join the lines together
            // if you put a blank line between the template and what it's modifying, you're a terrible person
            if(std::regex_match(trimmed, TemplateRegex)) {
                std::getline(stream, line);
                CommentExtractor::StripComments(line, isInBlockComment);
                trimmed += StrUtil::Trim(line);
            }

            if(m_Settings.ExtractTypes && TypeDataExtractor::IsAType(trimmed)) {
                auto type = TypeDataExtractor::Extract(trimmed, result.FileName, stream);
                result.Types.push_back(type);
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsAFunction(trimmed)) {
                result.FreeFunctions.push_back(FunctionDataExtractor::Extract(trimmed, stream, "", Visibility::PUBLIC));
            }
        }

        result.LineCount = nonBlankLines;
        return result;
    }
} // namespace Extractor