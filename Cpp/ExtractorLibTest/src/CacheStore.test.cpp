#include "TestCommon.h"

#include "Extractor/Cache/CacheStore.h"
#include "Extractor/Data/DefineData.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/Data/IncludeMap.h"
#include "Extractor/Workers/IncludeMapGenerator.h"

#include "Core/Utilities/StringUtils.h"
#include "Core/Utilities/TimeUtils.h"

#include <sstream>

namespace Extractor {
    struct CacheStoreTest : public ::testing::Test {
        void SetUp() {
            m_SharedStream = std::make_shared<std::stringstream>();
            store = std::make_unique<CacheStore>(m_SharedStream, m_SharedStream);

            sourceFile.FileName = "sourceFile.cpp";
            sourceFile.FilePath = "src/sourceFile.cpp";
            sourceFile.IncludeFiles.insert("headerFile.h");

            headerFile.FileName = "headerFile.h";
            headerFile.FilePath = "inc/headerFile.h";
        }

        CacheResult RoundTrip() {
            store->WriteCache(fileData, defineData, includeMap);
            return store->ReadCache();
        }

        std::shared_ptr<std::stringstream> m_SharedStream{nullptr};

        std::vector<FileData> fileData;
        DefineData defineData;
        IncludeMap includeMap;
        std::unique_ptr<CacheStore> store{nullptr};
        FileData sourceFile;
        FileData headerFile;
    };

    TEST_F(CacheStoreTest, WriteCache_WithNoData_WritesTime) {
        auto result = RoundTrip();

        ASSERT_TRUE(std::filesystem::file_time_type::clock::now() - result.CacheTime < std::chrono::duration(std::chrono::seconds(1)));
    }

    TEST_F(CacheStoreTest, ReadCache_WithDefineData_HasDefineData) {
        fileData.push_back(sourceFile);
        includeMap = GenerateIncludeMap(fileData);

        defineData.Defines["TEST"] = "1";
        defineData.DefineSource["TEST"] = sourceFile.FilePath;

        auto result = RoundTrip();

        ASSERT_EQ(result.Defines.size(), 1);
        ASSERT_EQ(result.Defines[sourceFile.FilePath]["TEST"], "1");
    }

    TEST_F(CacheStoreTest, ReadCache_WithIncludeData_HasIncludeData) {
        fileData.push_back(sourceFile);
        fileData.push_back(headerFile);

        includeMap = GenerateIncludeMap(fileData);

        auto result = RoundTrip();

        auto& includes = result.FileData.at(sourceFile.FilePath).IncludeFiles;
        ASSERT_EQ(includes.size(), 1);
        ASSERT_NE(includes.find(headerFile.FileName), includes.end());

        auto& includedBy = result.IncludedByMap;
        ASSERT_EQ(includedBy.size(), 2);
        ASSERT_NE(includedBy.at(headerFile.FilePath).find(sourceFile.FileName), includedBy.at(headerFile.FilePath).end());
        ASSERT_EQ(includedBy.at(sourceFile.FilePath).size(), 0);
    }
} // namespace Extractor