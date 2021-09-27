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

namespace {
    std::regex IncludeRegex("^#include [\"<]([^\">]+)[\">]$");

    std::string GetIncludeGuardName(const std::string& fileName) {
        return StrUtil::ToUpper(fileName.substr(0, fileName.find_first_of('.')));
    }
} // namespace

namespace Extractor {
    void FilePreProcessor::Execute(std::unordered_map<std::string, std::string>& knownDefines, std::unordered_set<std::string>& processedFiles) const {
        if(processedFiles.find(m_FilePath) != processedFiles.end()) {
            return;
        }
        processedFiles.insert(m_FilePath);

        const auto includeGuardName = GetIncludeGuardName(PathUtils::GetFileName(m_FilePath));

        if(!m_Stream) {
            Require::True(FileUtils::Exists(m_FilePath));
            m_Stream = FileUtils::OpenForRead(m_FilePath);
        }

        IfDefExtractor ifdefExtractor(knownDefines, *m_Stream);
        u8 ifDefDepth = 0;

        std::string line;
        std::smatch match;
        u16 newDefines{0};

        while(LineFetcher::GetNextLine(*m_Stream, line)) {
            if(std::regex_search(line, match, IncludeRegex)) {
                const auto lowerHeader = StrUtil::ToLower(match[1]);
                if(m_HeaderToFileMap->find(lowerHeader) != m_HeaderToFileMap->end()) {
                    const auto includeFilePath = m_HeaderToFileMap->at(lowerHeader);
                    FilePreProcessor fpp{includeFilePath, *m_HeaderToFileMap};
                    fpp.Execute(knownDefines, processedFiles);
                }
            } else if(ifdefExtractor.CanExtract(line)) {
                ifdefExtractor.Extract(line);
            } else if(DefineExtractor::CanExtract(line)) {
                const auto& [key, value] = DefineExtractor::Extract(line);
                knownDefines[key] = value;
                newDefines++;
            }
        }

        LOG_INFO(StrUtil::Format("Found %d defines for %s", newDefines, m_FilePath));
    }
} // namespace Extractor