#include "Extractor/Cache/CachePurge.h"
#include "Extractor/Cache/CacheStore.h"
#include "Extractor/Data/DefineData.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
#include "Extractor/FilePreProcessor.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Report/ExcelReport.h"
#include "Threading/Runner.h"
#include "TypeArgParse.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/TimeUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

Log::StdOutLogWriter logWriter{};
using namespace Extractor;
using namespace TypeCounter;

namespace {
    static constexpr ExtractorSettings Settings{false, true, true};

    std::unordered_map<std::string, std::string> GenerateHeaderFileMappings(const std::vector<std::string>& filePaths) {
        std::unordered_map<std::string, std::string> result{};
        for(const auto& file: filePaths) {
            auto toLower = StrUtil::ToLower(file);
            auto shortName = toLower.substr(toLower.find("inc") + 4); // skip inc and the path separator
            result[shortName] = file;
        }

        return result;
    }

    DefineData BuildDefineData(const std::unordered_map<std::string, std::string>& userDefines) {
        DefineData result;
        result.Defines.insert(userDefines.begin(), userDefines.end());
        for(const auto& [define, value]: userDefines) {
            result.DefineSource[define] = "UserProvided";
        }

        return result;
    }

    void PreProcessFiles(const std::vector<std::string>& filePaths, const CacheResult& cache, DefineData& defines) {
        auto headerMapping = GenerateHeaderFileMappings(filePaths);
        std::unordered_set<std::string> processedFiles{};

        for(const auto& filePath: filePaths) {
            FilePreProcessor fpp{filePath, headerMapping};
            fpp.Execute(cache, defines, processedFiles);
        }
    }

    std::vector<FileData> GatherFileData(const std::vector<std::string>& fileNames, const CacheResult& cache, const DefineData& defines) {
        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        std::transform(fileNames.begin(), fileNames.end(), std::back_inserter(jobs), [&cache, &defines](const std::string& file) { return std::make_unique<FileDataExtractor>(file, cache, defines, Settings); });

        // large code bases tend to have bigger files which take longer to parse
        Threading::ExpectedRunTime expectedRuntime = jobs.size() < 100 ? Threading::ExpectedRunTime::MILLISECONDS : Threading::ExpectedRunTime::SECONDS;
        return Runner::Get().RunAll(expectedRuntime, jobs);
    }

    std::string GetFilePrefix(std::string targetFileOption) {
        if(targetFileOption.empty()) {
            return TimeUtils::TodayNowLocalToString("%Y_%m_%d_%H_%M_%S");
        } else {
            return targetFileOption;
        }
    }

} // namespace

int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        auto fileNames = argParse.GetFileNames();
        Require::NotEmpty(fileNames, "Did not locate any filenames");

        auto cacheStore = CacheStore("TestCache.txt");
        auto cache = cacheStore.ReadCache();
        Extractor::Cache::Purge(cache, fileNames);

        auto defines = BuildDefineData(argParse.GetDefines());

        PreProcessFiles(fileNames, cache, defines);

        auto files = GatherFileData(fileNames, cache, defines);

        auto report = Report::ExcelReport(files);

        report.WriteReport(GetFilePrefix(argParse.GetTargetFile()));

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}