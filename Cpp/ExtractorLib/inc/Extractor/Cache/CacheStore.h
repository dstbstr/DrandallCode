#pragma once

#include "Extractor/Data/CacheResult.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Extractor {
    struct FileData;
    struct DefineData;
    struct IncludeMap;

    static constexpr char UserProvidedDefineName[]{"UserProvided"};

    class CacheStore {
    public:
        CacheStore(std::string cacheFileName);
        CacheStore(std::shared_ptr<std::istream> inStream, std::shared_ptr<std::ostream> outStream);
        void WriteCache(const std::vector<FileData>& fileData, const DefineData& defines, const IncludeMap& includes);
        CacheResult ReadCache();

    private:
        std::string m_FilePath;
        std::shared_ptr<std::istream> m_InStream;
        std::shared_ptr<std::ostream> m_OutStream;
    };
} // namespace Extractor