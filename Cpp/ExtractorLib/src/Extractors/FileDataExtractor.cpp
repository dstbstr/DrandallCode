#include "Extractor/FileDataExtractor.h"

#include "Extractor/BodyCount.h"
#include "Extractor/Data/Visibility.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/NamespaceExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Extractor/TypeDataExtractor.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/PathUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtils.h"

#include <fstream>
#include <regex>

namespace {
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");
    std::regex CloseBlockRegex("^[\\} ]+$");
} // namespace

// TODO: Add functionality for global variables?

namespace Extractor {

    FileData FileDataExtractor::Execute() const {
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

        // ScopedTimer timer(result.FileName + " Extract File", ScopedTimer::TimeUnit::MINUTE);
        // ScopedTimer timer(result.FileName + " Extract File");
        u64 nonBlankLines = 0;
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
            if(m_Settings.ExtractTypes && TypeDataExtractor::IsAType(line, match)) {
                auto type = TypeDataExtractor::Extract(match, result.FileName, namespaceExtractor.GetNamespace(), stream);
                result.Types.push_back(type);
                nonBlankLines += type.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsAFunction(line, match)) {
                auto functionData = FunctionDataExtractor::ExtractFunction(line, match, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                if(functionData.ClassName.empty()) {
                    result.FreeFunctions.push_back(functionData);
                }
                nonBlankLines += functionData.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsSpecialFunction(line, match)) {
                // inline constructor, should have already been declared, just need to skip through it
                auto function = FunctionDataExtractor::ExtractSpecialFunction(line, match, stream, "", Visibility::PUBLIC);
                nonBlankLines += function.LineCount - 1;
            } else if(m_Settings.ExtractFunctions && FunctionDataExtractor::IsOperatorOverload(line, match)) {
                auto function = FunctionDataExtractor::ExtractOperatorOverload(line, match, stream, namespaceExtractor.GetNamespace(), "", Visibility::PUBLIC);
                result.FreeOperatorOverloads.push_back(function);
                nonBlankLines += function.LineCount - 1;
            } else if(line[line.length() - 1] == '{') {
                // probably a map or array initializer
                namespaceExtractor.PushNestedCurly();
                nonBlankLines += BodyCount::GetBodyCount(line, stream);
            } else if(line[line.length() - 1] == '}') {
                auto closeBraces = std::count(line.begin(), line.end(), '}');
                closeBraces -= std::count(line.begin(), line.end(), '{');
                for(int i = 0; i < closeBraces; i++) {
                    try {
                        namespaceExtractor.PopNamespace();
                    } catch(std::exception e) {
                        LOG_WARN(StrUtil::Format("Failed to pop namespace.  File %s, NonBlankLine %u", result.FileName, nonBlankLines));
                    }
                }
            }
        }

        result.LineCount = nonBlankLines;
        return result;
    }
} // namespace Extractor