#include "ArgParse.h"
#include "FileData.h"
#include "IncludeCountTask.h"
#include "Instrumentation/Log.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Platform/Types.h"
#include "Threading/IRunnable.h"
#include "Threading/Runner.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/StringUtilities.h"

#include <chrono>
#include <vector>


inline std::chrono::steady_clock::time_point Now() {
    return std::chrono::high_resolution_clock::now();
}

long long DiffMicros(std::chrono::steady_clock::time_point mark) {
    return std::chrono::duration_cast<std::chrono::microseconds>(Now() - mark).count();
}

long long DiffMillis(std::chrono::steady_clock::time_point mark) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(Now() - mark).count();
}

Log::StdOutLogWriter logWriter{};

int main(int argc, char* argv[]) {
    // construct an argument parser with argc and argv
    // pass the root directory and the recurse option to a directory navigator
    // fan out the files to find include directives in each file
    // construct including and included maps
    ScopedTimer executionTimer("Total Runtime");
    ArgParse argParse(argc, argv);
    if(!argParse.ShouldParse()) {
        return 0;
    }

    std::vector<std::string> fileNames;
    {
        ScopedTimer timer("Gather filenames");
        fileNames = argParse.GetFileNames();
    }

    std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
    {
        ScopedTimer timer("Construct jobs");
        for(auto&& file: fileNames) {
            jobs.push_back(std::move(std::make_unique<IncludeCountTask>(file)));
        }
    }

    auto runner = Runner::Get();
    std::vector<FileData> result;
    {
        ScopedTimer timer("Run all jobs");
        result = runner.RunAll(jobs);
    }

    std::cin.ignore(1, '\n');
    return 0;
}