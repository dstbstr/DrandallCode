#include "All.h"

#include "Common.h"
#include "Utilities/StringUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"
#include "Threading/Runner.h"
#include "Threading/IRunnable.h"

#include "Constexpr/ConstexprRegex.h"

#include <iostream>
#include <charconv>
#include <fstream>
#include <array>
#include <filesystem>

Log::MinimalStdOutLogWriter logWriter{};
static std::unordered_map<std::string, std::chrono::microseconds> SolutionTimings;

std::vector<std::string> ReadInputFile(size_t year, size_t day) {
    std::vector<std::string> result{};
    auto targetFile = std::filesystem::current_path() / "Inputs" / ToString(year) / ("d" + ToString(day) + ".txt");
    auto stream = std::ifstream(targetFile.c_str());
    if(!stream.is_open()) {
        return {};
    }

    std::string line;
    while(std::getline(stream, line)) {
        result.push_back(line);
    }

    return result;
}

bool Check(size_t year, size_t day, bool hideOutput = false) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }

    auto GatherTiming = [year, day](std::chrono::microseconds elapsed) {
        auto key = Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":Test";
        SolutionTimings[key] = elapsed;
        };

    if (auto testTime = ScopedTimer(GatherTiming, "Test Time"); GetTests()[year][day]()) {
        if(!hideOutput) std::cout << "Test Pass\n";
        return true;
    }
    else {
        if(!hideOutput) std::cout << "Test Fail. :(\n";
        return false;
    }
}

struct PuzzleSolver : IRunnable<bool> {
    PuzzleSolver(size_t year, size_t day, bool hideOutput) : Year(year), Day(day), HideOutput(hideOutput) {}
    bool Execute() const override {
        if (!HideOutput) std::cout << "### " << Year << " Day " << Day << "###\n";
        if (!Check(Year, Day, HideOutput)) return false;

        const auto lines = ReadInputFile(Year, Day);
        for (const auto& [part, func] : GetSolutions()[Year][Day]) {
            auto GatherTiming = [=](std::chrono::microseconds elapsed) {
                auto key = Constexpr::ToString(Year) + "/" + Constexpr::ToString(Day) + ":" + Constexpr::ToString(part);
                SolutionTimings[key] = elapsed;
                };
            auto partTime = ScopedTimer(GatherTiming);
            auto result = func(lines);
            if (!HideOutput) {
                std::cout << "Part " << part << ": " << result << "\n";

                if (!GET_LOGS().empty()) {
                    std::cout << "## Logs ##\n";
                    std::cout << Constexpr::JoinVec("\n", GET_LOGS());
                    GET_LOGS().clear();
                }
            }
        }
        if (!HideOutput) {
            std::cout << "\n";
        }

        return true;
    }

    size_t Year;
    size_t Day;
    bool HideOutput;
};

void RunOne(size_t year, size_t day) {
    auto solver = PuzzleSolver(year, day, false);
    solver.Execute();
}

void RunYear(size_t year) {
    auto tasks = std::vector<std::unique_ptr<IRunnable<bool>>>{};
    for (const auto& [day, part] : GetSolutions()[year]) {
        tasks.push_back(std::move(std::make_unique<PuzzleSolver>(year, day, true)));
    }
    Runner::Get().RunAll(Threading::ExpectedRunTime::SECONDS, tasks);
}

void RunAll() {
    auto tasks = std::vector<std::unique_ptr<IRunnable<bool>>>{};
    for (const auto& [year, days] : GetSolutions()) {
        for (const auto& [day, parts] : days) {
            tasks.push_back(std::move(std::make_unique<PuzzleSolver>(year, day, true)));
        }
    }
    Runner::Get().RunAll(Threading::ExpectedRunTime::SECONDS, tasks);
}

void PrintTimings(size_t maxResults = 0, std::chrono::microseconds minElapsed = std::chrono::microseconds(0)) {
    std::vector<std::pair<std::chrono::microseconds, std::string>> timings;
    for (const auto& [key, elapsed] : SolutionTimings) {
        timings.push_back(std::make_pair(elapsed, key));
    }
    std::sort(timings.begin(), timings.end(), [](auto lhs, auto rhs) { return rhs.first < lhs.first; });

    size_t resultId = 0;
    for (auto [elapsed, key] : timings) {
        if (minElapsed > std::chrono::microseconds(0) && elapsed < minElapsed) break;
        if (maxResults > 0 && resultId > maxResults) break;

        std::cout << key << ": " << TimeUtils::DurationToString(elapsed) << "\n";
    }
}
//maybe look closer at this for more shenanigans: https://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char

int main(int, char**) {
    Constexpr::Regex::RunTest();
    //RunAll();
    //RunYear(2022);
    RunOne(2022, 25);
    //RunLatest();

    //PrintTimings(0, std::chrono::seconds(1));
    PrintTimings();
    return 0;
}
