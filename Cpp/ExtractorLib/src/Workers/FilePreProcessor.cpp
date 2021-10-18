#include "Extractor/FilePreProcessor.h"

#include "Extractor/Data/CacheResult.h"
#include "Extractor/Data/DefineData.h"
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
    void FilePreProcessor::Execute(const CacheResult& cache, DefineData& knownDefines, std::unordered_set<std::string>& processedFiles) const {
        if(processedFiles.find(m_FilePath) != processedFiles.end()) {
            return;
        }
        processedFiles.insert(m_FilePath);

        const auto& cacheDefines = cache.Defines;
        if(cacheDefines.find(m_FilePath) != cacheDefines.end()) {
            knownDefines.Defines.insert(cacheDefines.at(m_FilePath).begin(), cacheDefines.at(m_FilePath).end());
            for(const auto& [define, value]: cacheDefines.at(m_FilePath)) {
                knownDefines.DefineSource[define] = m_FilePath;
            }

            return;
        }

        const auto includeGuardName = GetIncludeGuardName(PathUtils::GetFileName(m_FilePath));

        if(!m_Stream) {
            Require::True(FileUtils::Exists(m_FilePath));
            m_Stream = FileUtils::OpenForRead(m_FilePath);
        }

        IfDefExtractor ifdefExtractor(knownDefines, *m_Stream);

        std::string line;
        std::smatch match;
        u16 newDefines{0};

        while(LineFetcher::GetNextLine(*m_Stream, line)) {
            if(std::regex_search(line, match, IncludeRegex)) {
                const auto lowerHeader = StrUtil::ToLower(match[1]);
                if(m_HeaderToFileMap->find(lowerHeader) != m_HeaderToFileMap->end()) {
                    const auto includeFilePath = m_HeaderToFileMap->at(lowerHeader);
                    FilePreProcessor fpp{includeFilePath, *m_HeaderToFileMap};
                    fpp.Execute(cache, knownDefines, processedFiles);
                }
            } else if(ifdefExtractor.CanExtract(line)) {
                ifdefExtractor.Extract(line);
            } else if(DefineExtractor::CanExtract(line)) {
                const auto& [key, value] = DefineExtractor::Extract(line);
                // let's not add the header guard to known defines.  Otherwise our IfDefExtractor will skip the file on the second pass
                if(key.find(includeGuardName) == key.npos) {
                    knownDefines.Defines[key] = value;
                    knownDefines.DefineSource[key] = m_FilePath;
                    newDefines++;
                }
            }
        }
    }
} // namespace Extractor