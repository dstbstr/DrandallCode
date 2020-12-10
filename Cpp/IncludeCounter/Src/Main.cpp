#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
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

template<class T>
struct IRunnable;

Log::StdOutLogWriter logWriter{};
using namespace IncludeCounter;
using namespace Extractor;

int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        std::vector<std::string> fileNames = argParse.GetFileNames();

        ExtractorSettings settings;
        settings.CountIncludes = true;
        settings.ExtractFunctions = false;
        settings.ExtractTypes = false;
        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<FileDataExtractor>(file, settings)));
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