#include "Extractor/FileDataExtractor.h"

#include "Extractor/CommentExtractor.h"
#include "Extractor/Data/Visibility.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/NamespaceExtractor.h"
#include "Extractor/TypeDataExtractor.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/StringUtilities.h"

#include <fstream>
#include <regex>

namespace {
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
    std::regex TemplateRegex("^template<[^>]*>$");
    std::regex CloseBlockRegex("^[\\} ]+$");
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
        NamespaceExtractor namespaceExtractor;

        u64 nonBlankLines = 0;
        u64 ignoredLines = 0;
        while(std::getline(stream, line)) {
            CommentExtractor::StripComments(line, isInBlockComment);
            auto trimmed = StrUtil::Trim(line);
            if(trimmed == "") { // do not check isInBlockComment here.  Line may end with a block comment, but start with code
                continue;
            }
            nonBlankLines++;
            if(std::regex_search(trimmed, match, IncludeRegex)) {
                if(m_Settings.CountIncludes) {
                    result.IncludeFiles.insert(match[1]);
                }
                continue;
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

            if(namespaceExtractor.IsNamespace(trimmed)) {
                namespaceExtractor.ExtractNamespace(trimmed);
            }
            if(m_Settings.ExtractTypes && TypeDataExtractor::IsAType(trimmed)) {
                auto type = TypeDataExtractor::Extract(trimmed, result.FileName, namespaceExtractor.GetNamespace(), stream);
                result.Types.push_back(type);
                nonBlankLines += type.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsAFunction(trimmed)) {
                auto functionData = FunctionDataExtractor::ExtractFunction(trimmed, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                if(!functionData.IsTemplated) {
                    // template definitions have to be defined in the header file
                    // we're potentially removing free template functions though :/
                    result.FreeFunctions.push_back(functionData);
                }
                nonBlankLines += functionData.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsSpecialFunction(trimmed)) {
                // inline constructor, should have already been declared, just need to skip through it
                auto function = FunctionDataExtractor::ExtractSpecialFunction(trimmed, stream, "", Visibility::PUBLIC);
                nonBlankLines += function.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsOperatorOverload(trimmed)) {
                auto function = FunctionDataExtractor::ExtractOperatorOverload(trimmed, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                result.FreeOperatorOverloads.push_back(function);
                nonBlankLines += function.LineCount - 1;
            } else if(std::regex_search(trimmed, CloseBlockRegex)) {
                auto closeBraces = std::count(trimmed.begin(), trimmed.end(), '}');
                for(int i = 0; i < closeBraces; i++) {
                    try {
                        namespaceExtractor.PopNamespace();
                    } catch(std::exception e) { LOG_WARN(StrUtil::Format("Failed to pop namespace.  File %s, NonBlankLine %u", result.FileName, nonBlankLines)); }
                }
            } else {
                ignoredLines++; // just for debugging
            }
        }

        result.LineCount = nonBlankLines;
        return result;
    }
} // namespace Extractor