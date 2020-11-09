#include "ArgParse.h"
//#include "IncludeCountTask.h"
#include "IncludeCounter/Data/FileData.h"
#include "IncludeCounter/Extractors/FileDataExtractor.h"
#include "IncludeCounter/IncludeMapGenerator.h"
#include "IncludeCounter/ResultGenerator.h"
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

int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        auto sortOrder = ResultGenerator::GetSortOrder((u8)argParse.GetSortOrderIndex()); // validate this early

        std::vector<std::string> fileNames = argParse.GetFileNames();

        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<Extractors::FileDataExtractor>(file)));
        }

        std::vector<FileData> files = Runner::Get().RunAll(jobs);

        IncludeMapGenerator(files).Generate();
        if(argParse.GetTargetFile().empty()) {
            ResultGenerator(files).PrintResultToStream(std::cout, sortOrder, argParse.IsDescending());
        } else {
            auto path = std::filesystem::path(argParse.GetTargetFile());
            std::filesystem::remove(path); // clear out old results
            auto stream = std::ofstream(path);
            ResultGenerator(files).PrintResultToStream(stream, sortOrder, argParse.IsDescending());
        }
        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}