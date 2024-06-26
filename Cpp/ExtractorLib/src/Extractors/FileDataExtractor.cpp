#include "Extractor/FileDataExtractor.h"

#include "Extractor/Data/CacheResult.h"
#include "Extractor/Data/DefineData.h"
#include "Extractor/Data/Visibility.h"
#include "Extractor/FilePreProcessor.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/BodyCount.h"
#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Extractor/Private/NamespaceExtractor.h"
#include "Extractor/TypeDataExtractor.h"

#include "Core/Instrumentation/Logging.h"
#include "Core/Utilities/Format.h"
#include "Core/Utilities/PathUtils.h"
#include "Core/Utilities/ScopedTimer.h"
#include "Core/Utilities/StringUtils.h"

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
        bool isHeader = m_FilePath[m_FilePath.length() - 1] == 'h';

        const FileData* cacheData = nullptr;
        if(m_Cache->FileData.find(m_FilePath) != m_Cache->FileData.end()) {
            cacheData = &m_Cache->FileData.at(m_FilePath);
        }

        if(cacheData) {
            result.LineCount = cacheData->LineCount;
            if(m_Settings.CountIncludes) {
                result.IncludeFiles.insert(cacheData->IncludeFiles.begin(), cacheData->IncludeFiles.end());
            }
            if(!isHeader) { // we only process cpp files for includes, and we've collected that data from the cache
                return result;
            }
        }

        std::ifstream stream(m_FilePath, std::ifstream::in);
        if(!stream.is_open()) {
            return result;
        }

        std::string line;
        std::smatch match;
        NamespaceExtractor namespaceExtractor;
        IfDefExtractor ifdefExtractor(*m_Defines, stream);

        u64 nonBlankLines = 0;
        while(LineFetcher::GetNextLine(stream, line)) {
            nonBlankLines++;
            if(ifdefExtractor.CanExtract(line)) {
                result.PreProcessorCount++;
                ifdefExtractor.Extract(line);
                continue;
            }

            if(std::regex_search(line, match, IncludeRegex)) {
                if(m_Settings.CountIncludes && !cacheData) {
                    result.IncludeFiles.insert(match[1]);
                }
                continue;
            }

            // only process non-header files if we're counting includes
            if(!(isHeader || m_Settings.CountIncludes)) {
                continue;
            }

            if(namespaceExtractor.IsNamespace(line)) {
                namespaceExtractor.ExtractNamespace(line);
                if(line.empty()) {
                    continue;
                }
            }
            if(m_Settings.ExtractTypes && TypeDataExtractor::IsAType(line, match)) {
                auto type = TypeDataExtractor::Extract(match, result.FileName, namespaceExtractor.GetNamespace(), *m_Defines, stream);
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
                    } catch(std::exception&) {
                        Log::Warn(StrUtil::Format("Failed to pop namespace.  File %s, NonBlankLine %u", result.FileName, nonBlankLines));
                    }
                }
            }
        }

        if(!cacheData) {
            result.LineCount = nonBlankLines;
        }
        return result;
    }
} // namespace Extractor