#include "CombinedReport.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/Data/PreProcessorResult.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
#include "Extractor/FilePreProcessor.h"
#include "FunctionReport.h"
#include "IReport.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Threading/Runner.h"
#include "TypeArgParse.h"
#include "TypeReport.h"
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

using ReportCollection = std::vector<std::pair<std::unique_ptr<IReport>, std::string>>;

namespace {
    static constexpr ExtractorSettings Settings{false, true, true};

    std::unordered_map<std::string, PreProcessorResult> PreProcessFiles(const std::vector<std::string>& fileNames, const std::vector<std::string>& userDefines) {
        std::vector<std::unique_ptr<IRunnable<PreProcessorResult>>> preProcessingJobs;
        std::transform(fileNames.begin(), fileNames.end(), std::back_inserter(preProcessingJobs), [&userDefines](const std::string& file) { return std::make_unique<FilePreProcessor>(file, userDefines); });
        auto preProcessedResults = Runner::Get().RunAll(Threading::ExpectedRunTime::MILLISECONDS, preProcessingJobs);

        std::unordered_map<std::string, PreProcessorResult> result;
        for(auto&& file: preProcessedResults) {
            result[file.FileName] = file;
        }

        return result;
    }

    std::vector<FileData> GatherFileData(const std::vector<std::string>& fileNames, const std::vector<std::string>& userDefines, std::unordered_map<std::string, PreProcessorResult>& preProcessedFileData) {
        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        std::transform(fileNames.begin(), fileNames.end(), std::back_inserter(jobs), [&userDefines, &preProcessedFileData](const std::string& file) { return std::make_unique<FileDataExtractor>(file, userDefines, preProcessedFileData, Settings); });

        // large code bases tend to have bigger files which take longer to parse
        Threading::ExpectedRunTime expectedRuntime = jobs.size() < 100 ? Threading::ExpectedRunTime::MILLISECONDS : Threading::ExpectedRunTime::SECONDS;
        return Runner::Get().RunAll(expectedRuntime, jobs);
    }

    ReportCollection GenerateReports(std::vector<FileData> results, bool functionReport, bool typeReport) {
        ReportCollection reports;
        reports.push_back(std::make_pair(std::make_unique<CombinedReport>(results), "Combined"));

        if(functionReport) {
            reports.push_back(std::make_pair(std::make_unique<FunctionReport>(results), "Function"));
        }
        if(typeReport) {
            reports.push_back(std::make_pair(std::make_unique<TypeReport>(results), "Types"));
        }

        return reports;
    }

    std::string GetFilePrefix(std::string targetFileOption) {
        if(targetFileOption.empty()) {
            return TimeUtils::TodayNowToString("%Y_%m_%d_%H_%M_%S");
        } else {
            return targetFileOption;
        }
    }

    void WriteReport(ReportCollection& reports, const std::string& filePrefix) {
        for(auto&& reportPair: reports) {
            auto path = std::filesystem::path(filePrefix + "_" + reportPair.second + ".csv"); // todo: get the suffix from the report
            std::filesystem::remove(path); // clear out any old results
            auto stream = std::ofstream(path);
            reportPair.first->PrintResultToStream(stream);
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
        auto userDefines = argParse.GetDefines();

        auto files = GatherFileData(fileNames, userDefines, PreProcessFiles(fileNames, userDefines));

        auto reports = GenerateReports(files, argParse.RunFunctionReport(), argParse.RunTypeReport());

        std::string filePrefix = GetFilePrefix(argParse.GetTargetFile());

        WriteReport(reports, filePrefix);

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}