#ifndef __FILEPREPROCESSOR_H__
#define __FILEPREPROCESSOR_H__

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Extractor {
    struct DefineData;
    struct CacheResult;
    
    class FilePreProcessor {
    public:
        FilePreProcessor(std::string filePath, const std::unordered_map<std::string, std::string>& headerToFileMap) : m_FilePath(filePath), m_Stream(nullptr), m_HeaderToFileMap(&headerToFileMap) {}
        FilePreProcessor(std::string filePath, const std::unordered_map<std::string, std::string>& headerToFileMap, std::shared_ptr<std::istream> stream) : m_FilePath(filePath), m_HeaderToFileMap(&headerToFileMap), m_Stream(stream) {}
        FilePreProcessor(const FilePreProcessor&) = delete;
        FilePreProcessor& operator=(const FilePreProcessor&) = delete;

        void Execute(const CacheResult& cache, DefineData& knownDefines, std::unordered_set<std::string>& processedFiles) const;

    private:
        std::string m_FilePath;
        const std::unordered_map<std::string, std::string>* m_HeaderToFileMap;
        mutable std::shared_ptr<std::istream> m_Stream;
    };
} // namespace Extractor

#endif // __FILEPREPROCESSOR_H__