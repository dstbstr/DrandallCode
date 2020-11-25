#include "CombinedReport.h"
#include "Extractor/Data/FileData.h"
#include "Extractor/ExtractorSettings.h"
#include "Extractor/FileDataExtractor.h"
#include "FunctionReport.h"
#include "IReport.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Threading/Runner.h"
#include "TypeArgParse.h"
#include "TypeReport.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/TimeUtils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template<class T>
struct IRunnable;

Log::StdOutLogWriter logWriter{};
using namespace Extractor;
using namespace TypeCounter;

int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        std::vector<std::string> fileNames = argParse.GetFileNames();

        ExtractorSettings settings;
        settings.CountIncludes = false;
        settings.ExtractFunctions = true;
        settings.ExtractTypes = true;
        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<FileDataExtractor>(file, settings)));
        }

        std::vector<FileData> files = Runner::Get().RunAll(jobs);

        std::unique_ptr<IReport> report;
        std::vector<std::pair<std::unique_ptr<IReport>, std::string>> reports;
        reports.push_back(std::make_pair(std::make_unique<CombinedReport>(files), "Combined"));

        if(argParse.RunFunctionReport()) {
            reports.push_back(std::make_pair(std::make_unique<FunctionReport>(files), "Function"));
        }
        if(argParse.RunTypeReport()) {
            reports.push_back(std::make_pair(std::make_unique<TypeReport>(files), "Types"));
        }

        std::string filePrefix;
        if(argParse.GetTargetFile() == "") {
            filePrefix = TimeUtils::TodayNowToString("%Y_%m_%d_%H_%M_%S");
        } else {
            filePrefix = argParse.GetTargetFile();
        }

        for(auto&& reportPair: reports) {
            auto path = std::filesystem::path(filePrefix + "_" + reportPair.second + ".csv"); // todo: get the suffix from the report
            std::filesystem::remove(path); // clear out any old results
            auto stream = std::ofstream(path);
            reportPair.first->PrintResultToStream(stream);
        }

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}