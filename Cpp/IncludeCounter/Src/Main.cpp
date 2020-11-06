#include "ArgParse.h"
#include "FileData.h"
#include "IncludeCountTask.h"
#include "IncludeMapGenerator.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "ResultGenerator.h"

//#include "Threading/IRunnable.h"
#include "Threading/Runner.h"
#include "Utilities/Format.h"
#include "Utilities/ScopedTimer.h"

//#include "Utilities/StringUtilities.h"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

template<class T>
struct IRunnable;

Log::StdOutLogWriter logWriter{};

int main(int argc, char* argv[]) {
    // construct an argument parser with argc and argv
    // pass the root directory and the recurse option to a directory navigator
    // fan out the files to find include directives in each file
    // construct including and included maps
    ScopedTimer executionTimer("Total Runtime");
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        auto sortOrder = ResultGenerator::GetSortOrder((u8)argParse.GetSortOrderIndex()); //validate this early

        std::vector<std::string> fileNames = argParse.GetFileNames();

        std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<IncludeCountTask>(file)));
        }

        std::vector<FileData> files = Runner::Get().RunAll(jobs);

        IncludeMapGenerator(files).Generate();
        if(argParse.GetTargetFile().empty()) {
            ResultGenerator(files).PrintResultToStream(std::cout, sortOrder, argParse.IsDescending());
        } else {
            auto path = std::filesystem::path(argParse.GetTargetFile());
            std::filesystem::remove(path); //clear out old results
            auto stream = std::ofstream(path);
            ResultGenerator(files).PrintResultToStream(stream, sortOrder, argParse.IsDescending());
        }
        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}