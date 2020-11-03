#include "ArgParse.h"
#include "FileData.h"
#include "IncludeCountTask.h"
#include "Instrumentation/Log.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Platform/Types.h"
#include "Utilities/StringUtilities.h"

#include <chrono>
#include <ppltasks.h>
#include <thread>
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

    std::vector<IncludeCountTask> jobs;
    for(auto&& file: fileNames) {
        jobs.push_back(IncludeCountTask(file));
    }

    LOG_INFO(StrUtil::Format("Time to construct jobs: %d Microseconds", DiffMicros(mark)));
    mark = Now();

    // kick off the jobs
    static const u32 MaxConcurrency = std::thread::hardware_concurrency();
    std::vector<concurrency::task<FileData>> tasks;
    std::vector<concurrency::task<FileData>> completedTasks(jobs.size());

    LOG_INFO(StrUtil::Format("Max concurrency: %d", MaxConcurrency));
    while(!jobs.empty()) {
        for(u32 i = 0; i < MaxConcurrency && !jobs.empty(); i++) {
            tasks.push_back(jobs.back().CountIncludes());
            jobs.pop_back();
        }

        // work through all
        concurrency::when_all(tasks.begin(), tasks.end());
        for(auto&& completed: tasks) {
            completedTasks.push_back(completed);
        }
        tasks.clear();
    }

    concurrency::when_all(tasks.begin(), tasks.end());

    LOG_INFO(StrUtil::Format("Time to run all jobs: %dms", DiffMillis(mark)));
    mark = Now();

    LOG_INFO(StrUtil::Format("Total runtime: %dms", DiffMillis(start)));
    std::cin.ignore(1, '\n');
    return 0;
}