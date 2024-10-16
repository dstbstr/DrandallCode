#pragma once

#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"

#include <unordered_map>

namespace Extractor {
    struct DefineData;
    struct CacheResult;

    class FileDataExtractor {
    public:
        FileDataExtractor(std::string filePath, const CacheResult& cache, const DefineData& defines, ExtractorSettings settings) : m_FilePath(filePath), m_Cache(&cache), m_Defines(&defines), m_Settings(settings) {}
        FileDataExtractor(const FileDataExtractor&) = delete;
        FileDataExtractor& operator=(const FileDataExtractor&) = delete;

        FileData Execute() const;

    private:
        std::string m_FilePath;
        const DefineData* m_Defines;
        const CacheResult* m_Cache;
        ExtractorSettings m_Settings;
    };

} // namespace Extractor