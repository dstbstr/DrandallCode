#include "ArgParse.h"
#include "FileData.h"
#include "IncludeCountTask.h"
#include "Instrumentation/Log.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Platform/Types.h"
#include "Threading/IRunnable.h"
#include "Threading/Runner.h"
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

    auto start = Now();
    ArgParse argParse(argc, argv);
    LOG_INFO(StrUtil::Format("Time to parse args: %d Microseconds", DiffMicros(start)));
    auto mark = Now();
    if(!argParse.ShouldParse()) {
        return 0;
    }

    auto fileNames = argParse.GetFileNames();
    LOG_INFO(StrUtil::Format("Time to gather filenames: %d Microseconds", DiffMicros(mark)));
    mark = Now();

    std::vector<std::unique_ptr<IRunnable<FileData>>> jobs;
    for(auto&& file: fileNames) {
        jobs.push_back(std::move(std::make_unique<IncludeCountTask>(file)));
    }

    LOG_INFO(StrUtil::Format("Time to construct jobs: %d Microseconds", DiffMicros(mark)));
    mark = Now();

    // kick off the jobs
    auto runner = Runner::Get();
    auto result = runner.RunAll<FileData>(jobs);

    LOG_INFO(StrUtil::Format("Time to run all jobs: %dms", DiffMillis(mark)));
    mark = Now();

    LOG_INFO(StrUtil::Format("Total runtime: %dms", DiffMillis(start)));
    std::cin.ignore(1, '\n');
    return 0;
}