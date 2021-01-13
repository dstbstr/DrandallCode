#include "Extractor/Data/FileData.h"
#include "Extractor/Data/PreProcessorResult.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
#include "Extractor/FilePreProcessor.h"
#include "Extractor/Workers/IncludeMapGenerator.h"
#include "IncludeArgParse.h"
#include "IncludeReport.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
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
} // namespace

int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        auto fileNames = argParse.GetFileNames();
        auto userDefines = argParse.GetDefines();
        auto preProcessedFiles = PreProcessFiles(fileNames, userDefines);

        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<FileDataExtractor>(file, userDefines, preProcessedFiles, Settings)));
        }

        Threading::ExpectedRunTime expectedRunTime = jobs.size() < 100 ? Threading::ExpectedRunTime::MILLISECONDS : Threading::ExpectedRunTime::SECONDS;
        std::vector<FileData> files = Runner::Get().RunAll(expectedRunTime, jobs);
        IncludeMapGenerator(files).Generate();
        IncludeReport report(files);

        if(argParse.GetTargetFile().empty()) {
            report.PrintResultToStream(std::cout);
        } else {
            auto path = std::filesystem::path(argParse.GetTargetFile() + "_Includes.csv");
            std::filesystem::remove(path); // clear out old results
            auto stream = std::ofstream(path);
            report.PrintResultToStream(stream);
        }
        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}