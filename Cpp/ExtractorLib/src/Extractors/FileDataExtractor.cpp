#include "Extractor/FileDataExtractor.h"

#include "Extractor/Data/Visibility.h"
#include "Extractor/FilePreProcessor.h"
#include "Extractor/FunctionDataExtractor.h"
#include "Extractor/Private/BodyCount.h"
#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Extractor/Private/NamespaceExtractor.h"
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
        std::vector<std::string> knownDefines;
        IfDefExtractor ifdefExtractor(knownDefines, stream);

        u64 nonBlankLines = 0;
        while(LineFetcher::GetNextLine(stream, line)) {
            nonBlankLines++;
            if(ifdefExtractor.CanExtract(line)) {
                result.PreProcessorCount++;
                ifdefExtractor.Extract(line);
                continue;
            }

            if(std::regex_search(line, match, IncludeRegex)) {
                if(m_Settings.CountIncludes) {
                    result.IncludeFiles.insert(match[1]);
                }
                std::string includeName = PathUtils::GetFileName(match[1]);
                if(m_PreProcessedFiles->find(includeName) != m_PreProcessedFiles->end() && includeName != result.FileName) {
                    PreProcessorResult preProcessedFile = m_PreProcessedFiles->at(includeName);
                    if(preProcessedFile.HasConditionalDefines) {
                        FilePreProcessor preprocessor(preProcessedFile.FilePath);
                        preprocessor.Reprocess(preProcessedFile, knownDefines);
                    }
                    for(auto&& defineKV: preProcessedFile.Defines) {
                        knownDefines.push_back(defineKV.first); // TODO: later if we want to do more preprocess work, we'll want to keep the definition as well
                        ifdefExtractor.AddDefine(defineKV.first);
                    }
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
                auto type = TypeDataExtractor::Extract(match, result.FileName, namespaceExtractor.GetNamespace(), knownDefines, stream);
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
                    } catch(std::exception& e) {
                        LOG_WARN(StrUtil::Format("Failed to pop namespace.  File %s, NonBlankLine %u", result.FileName, nonBlankLines));
                    }
                }
            }
        }

        result.LineCount = nonBlankLines;
        return result;
    }
} // namespace Extractor