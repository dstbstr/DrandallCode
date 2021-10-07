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

    void PreProcessFiles(const std::vector<std::string>& filePaths, std::unordered_map<std::string, std::string>& defines) {
        auto headerMapping = GenerateHeaderFileMappings(filePaths);
        std::unordered_set<std::string> processedFiles{};

        for(const auto& filePath: filePaths) {
            FilePreProcessor fpp{filePath, headerMapping};
            fpp.Execute(defines, processedFiles);
        }
    }

    std::string GetFilePrefix(std::string targetFileOption) {
        if(targetFileOption.empty()) {
            return TimeUtils::TodayNowToString("%Y_%m_%d_%H_%M_%S");
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
        auto defines = argParse.GetDefines();
        PreProcessFiles(fileNames, defines);

        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<FileDataExtractor>(file, defines, Settings)));
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

        Report::ExcelReport report(files);
        report.WriteReport(GetFilePrefix(argParse.GetTargetFile()));

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}