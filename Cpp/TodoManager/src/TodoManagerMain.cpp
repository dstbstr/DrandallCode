#include "Threading/IRunnable.h"
#include "Threading/Runner.h"
#include "Todo/TodoReader.h"
#include "Todo/TodoReport.h"
#include "Todo/TodoSettings.h"
#include "TodoArgParse.h"
#include "Utilities/Require.h"
#include "Utilities/ScopedTimer.h"
#include "Utilities/TimeUtils.h"

#include <iostream>

using namespace TodoManager;
using namespace TodoLib;

namespace {
    std::vector<Todo> FindAllTodos(const std::vector<std::string>& fileNames) {
        std::vector<std::unique_ptr<IRunnable<std::vector<Todo>>>> jobs;
        auto settings = TodoSettings{};
        std::transform(fileNames.begin(), fileNames.end(), std::back_inserter(jobs), [&settings](const std::string& file) { return std::make_unique<TodoReader>(settings, file); });

        Threading::ExpectedRunTime expectedRuntime = jobs.size() < 100 ? Threading::ExpectedRunTime::MILLISECONDS : Threading::ExpectedRunTime::SECONDS;
        auto results = Runner::Get().RunAll(expectedRuntime, jobs);

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
} // namespace
int main(int argc, char* argv[]) {
    ScopedTimer executionTimer("Total Runtime");
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