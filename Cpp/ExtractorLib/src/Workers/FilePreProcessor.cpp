#include "Extractor/FilePreProcessor.h"

#include "Extractor/Private/DefineExtractor.h"
#include "Extractor/Private/IfDefExtractor.h"
#include "Extractor/Private/LineFetcher.h"
#include "Instrumentation/Log.h"
#include "Utilities/FileUtils.h"
#include "Utilities/Format.h"
#include "Utilities/PathUtils.h"
#include "Utilities/Require.h"
#include "Utilities/StringUtils.h"

#include <fstream>
#include <memory>

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

        if(!m_Stream) {
            Require::True(FileUtils::Exists(m_FilePath));
            m_Stream = FileUtils::OpenForRead(m_FilePath);
        }

        IfDefExtractor ifdefExtractor(m_UserDefines, *m_Stream);
        u8 ifDefDepth = 0;

        std::string line;

        while(LineFetcher::GetNextLine(*m_Stream, line)) {
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
                } else if(line.find(includeGuardName) == line.npos) {
                    auto keyAndValue = DefineExtractor::Extract(line);
                    result.Defines[keyAndValue.first] = keyAndValue.second;
                }
            }
        }

        LOG_INFO(StrUtil::Format("Finished FirstPass of %s", m_FilePath));
        if(result.Defines.empty()) {
            LOG_INFO("Found no defines on first pass");
        } else {
            for(auto&& define: result.Defines) {
                LOG_INFO(define.first + " = " + define.second);
            }
        }
        return result;
    }

    void FilePreProcessor::Reprocess(PreProcessorResult& initialResult, std::vector<std::string> knownDefines) const {
        LOG_INFO(StrUtil::Format("Reprocessing %s", initialResult.FileName));

        std::string includeGuardName = GetIncludeGuardName(initialResult.FileName);
        if(!m_Stream) {
            Require::True(FileUtils::Exists(m_FilePath));
            m_Stream = FileUtils::OpenForRead(m_FilePath);
        }

        std::vector<std::string> defines = knownDefines;
        defines.insert(defines.end(), m_UserDefines.begin(), m_UserDefines.end());

        IfDefExtractor ifdefExtractor(defines, *m_Stream);
        std::string line;

        while(LineFetcher::GetNextLine(*m_Stream, line)) {
            if(ifdefExtractor.CanExtract(line)) {
                ifdefExtractor.Extract(line);
                continue;
            }
            if(DefineExtractor::CanExtract(line)) {
                auto keyAndValue = DefineExtractor::Extract(line);
                initialResult.Defines[keyAndValue.first] = keyAndValue.second;
                LOG_INFO(StrUtil::Format("Found define: %s = %s", keyAndValue.first, keyAndValue.second));
            }
        }
    }
} // namespace Extractor