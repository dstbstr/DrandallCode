#include "ArgParse.h"
#include "FileData.h"
#include "IncludeCountTask.h"
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

int main(int argc, char* argv[]) {
    // construct an argument parser with argc and argv
    // pass the root directory and the recurse option to a directory navigator
    // fan out the files to find include directives in each file
    // construct including and included maps

    auto start = Now();
    ArgParse argParse(argc, argv);
    std::cout << "Time to parse args: " << DiffMicros(start) << " Microseconds" << std::endl;
    auto mark = Now();
    if(!argParse.ShouldParse()) {
        return 0;
    }

    auto fileNames = argParse.GetFileNames();
    std::cout << "Time to gather filenames: " << DiffMicros(mark) << " Microseconds" << std::endl;
    mark = Now();

    std::vector<IncludeCountTask> jobs;
    for(auto&& file: fileNames) {
        jobs.push_back(IncludeCountTask(file));
    }

    std::cout << "Time to construct jobs: " << DiffMicros(mark) << " Microseconds" << std::endl;
    mark = Now();

    // kick off the jobs
    static const u32 MaxConcurrency = std::thread::hardware_concurrency();
    std::vector<concurrency::task<FileData>> tasks;
    std::vector<concurrency::task<FileData>> completedTasks(jobs.size());

    std::cout << "Max concurrency: " << MaxConcurrency << std::endl;
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

    std::cout << "Time to run all jobs: " << DiffMillis(mark) << " Milliseconds" << std::endl;
    mark = Now();

    std::cout << "Done" << std::endl;
    std::cout << "Total runtime: " << DiffMillis(start) << "ms";
    std::cin.ignore(1, '\n');
    return 0;
}