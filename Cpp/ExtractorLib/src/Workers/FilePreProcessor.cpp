#include "Extractor/FilePreProcessor.h"

#include "Extractor/Private/DefineExtractor.h"
#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Instrumentation/Log.h"
#include "Utilities/Format.h"
#include "Utilities/PathUtils.h"
#include "Utilities/StringUtils.h"

#include <fstream>

namespace {
    std::string GetIncludeGuardName(const std::string& fileName) {
        return StrUtil::ToUpper(fileName.substr(0, fileName.find_first_of('.')));
    }
} // namespace

namespace Extractor {
    PreProcessorResult FilePreProcessor::Execute() const {
        PreProcessorResult result;
        result.FilePath = m_FilePath;
        result.FileName = PathUtils::GetFileName(m_FilePath);

        std::string includeGuardName = GetIncludeGuardName(result.FileName);

        std::ifstream stream(m_FilePath, std::ifstream::in);
        if(!stream.is_open()) {
            LOG_WARN(StrUtil::Format("Failed to open file: %s", m_FilePath));
            return result;
        }
        static std::vector<std::string> emptyDefineList = {};

        IfDefExtractor ifdefExtractor(emptyDefineList, stream);
        u8 ifDefDepth = 0;

        std::string line;

        while(LineFetcher::GetNextLine(stream, line)) {
            if(ifdefExtractor.CanExtract(line)) {
                if(line.find(includeGuardName) != line.npos) {
                    // include guard
                } else if(line == "#endif") {
                    ifDefDepth--;
                } else if(StrUtil::StartsWith(line, "#if")) {
                    ifDefDepth++;
                }
                continue;
            }

            if(DefineExtractor::CanExtract(line)) {
                if(ifDefDepth > 0) {
                    result.HasConditionalDefines = true;
                } else {
                    auto keyAndValue = DefineExtractor::Extract(line);
                    result.Defines[keyAndValue.first] = keyAndValue.second;
                }
            }
        }

        return result;
    }

    void FilePreProcessor::Reprocess(PreProcessorResult& initialResult, std::vector<std::string> knownDefines) const {
        std::string includeGuardName = GetIncludeGuardName(initialResult.FileName);
        std::ifstream stream(m_FilePath, std::ifstream::in);
        if(!stream.is_open()) {
            LOG_WARN(StrUtil::Format("Failed to open file: %s", m_FilePath));
            return;
        }
        IfDefExtractor ifdefExtractor(knownDefines, stream);
        std::string line;

        while(LineFetcher::GetNextLine(stream, line)) {
            if(ifdefExtractor.CanExtract(line)) {
                ifdefExtractor.Extract(line);
                continue;
            }
            if(DefineExtractor::CanExtract(line)) {
                auto keyAndValue = DefineExtractor::Extract(line);
                initialResult.Defines[keyAndValue.first] = keyAndValue.second;
            }
        }
    }
} // namespace Extractor