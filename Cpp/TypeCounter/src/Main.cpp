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
        if(argParse.RunFunctionReport()) {
            report = std::make_unique<FunctionReport>(files);
        } else {
            report = std::make_unique<TypeReport>(files);
        }

        if(argParse.GetTargetFile().empty()) {
            report->PrintResultToStream(std::cout);
        } else {
            auto path = std::filesystem::path(argParse.GetTargetFile());
            std::filesystem::remove(path); // clear out old results
            auto stream = std::ofstream(path);
            report->PrintResultToStream(stream);
        }
        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}