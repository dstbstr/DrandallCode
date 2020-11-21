#include "Extractor/FileDataExtractor.h"

#include "Extractor/Data/Visibility.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/NamespaceExtractor.h"
#include "Extractor/TypeDataExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/StringUtilities.h"

#include <fstream>
#include <regex>

namespace {
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
    std::regex CloseBlockRegex("^[\\} ]+$");
} // namespace

// TODO: Add functionality for global variables?

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
        NamespaceExtractor namespaceExtractor;

        u64 nonBlankLines = 0;
        u64 ignoredLines = 0;
        while(LineFetcher::GetNextLine(stream, line)) {
            nonBlankLines++;
            if(std::regex_search(line, match, IncludeRegex)) {
                if(m_Settings.CountIncludes) {
                    result.IncludeFiles.insert(match[1]);
                }
                continue;
            }
            if(!isHeader || (!m_Settings.ExtractTypes && !m_Settings.ExtractFunctions)) {
                continue;
            }

            if(namespaceExtractor.IsNamespace(line)) {
                namespaceExtractor.ExtractNamespace(line);
            }
            if(m_Settings.ExtractTypes && TypeDataExtractor::IsAType(line)) {
                auto type = TypeDataExtractor::Extract(line, result.FileName, namespaceExtractor.GetNamespace(), stream);
                result.Types.push_back(type);
                nonBlankLines += type.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsAFunction(line)) {
                auto functionData = FunctionDataExtractor::ExtractFunction(line, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                if(functionData.ClassName.empty()) {
                    result.FreeFunctions.push_back(functionData);
                }
                nonBlankLines += functionData.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsSpecialFunction(line)) {
                // inline constructor, should have already been declared, just need to skip through it
                auto function = FunctionDataExtractor::ExtractSpecialFunction(line, stream, "", Visibility::PUBLIC);
                nonBlankLines += function.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsOperatorOverload(line)) {
                auto function = FunctionDataExtractor::ExtractOperatorOverload(line, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                result.FreeOperatorOverloads.push_back(function);
                nonBlankLines += function.LineCount - 1;
            } else if(std::regex_search(line, CloseBlockRegex)) {
                auto closeBraces = std::count(line.begin(), line.end(), '}');
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