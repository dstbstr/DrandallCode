#include "Extractor/Cache/CachePurge.h"
#include "Extractor/Cache/CacheStore.h"
#include "Extractor/Data/DefineData.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
#include "Extractor/FilePreProcessor.h"
#include "Extractor/Workers/IncludeMapGenerator.h"
#include "IncludeArgParse.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Report/CsvReport.h"
#include "Report/ExcelReport.h"
#include "Threading/Runner.h"
#include "Utilities/ScopedTimer.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Log::StdOutLogWriter logWriter{};
using namespace IncludeCounter;
using namespace Extractor;

namespace {
    constexpr ExtractorSettings Settings{true /*countIncludes*/, false /*extractTypes*/, false /*extractFunctions*/};

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
            result.DefineSource[define] = UserProvidedDefineName;
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
        auto cacheStore = CacheStore("TestCache.txt");
        auto cache = cacheStore.ReadCache();
        Extractor::Cache::Purge(cache, fileNames);

        auto defines = BuildDefineData(argParse.GetDefines()); // TODO: if user defines have changed, does that invalidate the cache?
        PreProcessFiles(fileNames, cache, defines);

        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(const auto& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<FileDataExtractor>(file, cache, defines, Settings)));
        }

        Threading::ExpectedRunTime expectedRunTime = jobs.size() < 100 ? Threading::ExpectedRunTime::MILLISECONDS : Threading::ExpectedRunTime::SECONDS;
        std::vector<FileData> files = Runner::Get().RunAll(expectedRunTime, jobs);
        auto includeMap = GenerateIncludeMap(files);

        for(auto& file: files) {
            file.TotalIncludeCount = includeMap.Dependencies[file.FileName].size();
            file.IncludedByCount = includeMap.DependsOnMe[file.FileName].size();
            file.TotalLineCount = file.LineCount;
            for(const auto& include: includeMap.Dependencies[file.FileName]) {
                file.TotalLineCount += includeMap.LineCounts[include];
            }
        }

        cacheStore.WriteCache(files, defines, includeMap);
        Report::ExcelReport report(files);
        report.WriteReport(GetFilePrefix(argParse.GetTargetFile()));

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}