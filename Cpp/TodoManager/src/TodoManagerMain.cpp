#include "Todo/TodoReader.h"
#include "Todo/TodoReport.h"
#include "Todo/TodoSettings.h"
#include "TodoArgParse.h"

#include "Core/Instrumentation/Logging.h"
#include "Core/Instrumentation/ScopedTimer.h"
#include "Core/Utilities/Require.h"
#include "Core/Utilities/TimeUtils.h"
#include "Core/Threading/Tasks.h"

#include <iostream>

using namespace TodoManager;
using namespace TodoLib;

namespace {
    std::vector<Todo> FindAllTodos(const std::vector<std::string>& fileNames) {
        TodoSettings settings;
        std::vector<std::function<std::vector<Todo>()>> jobs;
        for (const auto& file : fileNames) {
            jobs.emplace_back([&settings, file]() { return TodoReader(settings, file).Execute(); });
        }
        auto results = WhenAll(jobs).get();

        std::vector<Todo> result{};
        for(const auto& list: results) {
            result.insert(result.end(), list.begin(), list.end());
        }

        return result;
    }

    std::string GetFilePrefix(std::string targetFileOption) {
        if(targetFileOption.empty()) {
            return TimeUtils::TodayNowLocalToString("%Y_%m_%d_%H_%M_%S");
        } else {
            return targetFileOption;
        }
    }

    void LogTime(std::string_view label, std::chrono::microseconds elapsed) {
        Log::Info(std::format("{}: {}ms", label, elapsed.count() / 1000));
    }
} // namespace
int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime", LogTime);
    try {
        ArgParse argParse(argc, argv);
        if(!argParse.ShouldParse()) {
            return 0;
        }
        auto fileNames = argParse.GetFileNames();
        Require::NotEmpty(fileNames, "Did not locate any filenames");

        auto todos = FindAllTodos(fileNames);

        TodoReportGenerator::WriteToExcel(todos, GetFilePrefix(argParse.GetTargetFile()));

        return 0;
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }
}