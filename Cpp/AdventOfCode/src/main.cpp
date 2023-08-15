#include "All.h"

#include "Common.h"
#include "Utilities/StringUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"

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

bool Check(size_t year, size_t day) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }

    auto GatherTiming = [year, day](std::chrono::microseconds elapsed) {
        auto key = Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":Test";
        SolutionTimings[key] = elapsed;
        };

    if (auto testTime = ScopedTimer(GatherTiming, "Test Time"); GetTests()[year][day]()) {
        std::cout << "Test Pass\n";
        return true;
    }
    else {
        std::cout << "Test Fail. :(\n";
        return false;
    }
}

void RunOne(size_t year, size_t day) {
    std::cout << "### " << year << " Day " << day << " ###\n";
    if (!Check(year, day)) return;

    const auto lines = ReadInputFile(year, day);
    for (const auto& [part, func] : GetSolutions()[year][day]) {
        auto GatherTiming = [year, day, part](std::chrono::microseconds elapsed) {
            auto key = Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":" + Constexpr::ToString(part);
            SolutionTimings[key] = elapsed;
            };
        auto partTime = ScopedTimer(GatherTiming);
        std::cout << "Part " << part << ": " << func(lines) << "\n";

        if (!GET_LOGS().empty()) {
            std::cout << "## Logs ##\n";
            std::cout << Constexpr::JoinVec("\n", GET_LOGS());
            GET_LOGS().clear();
        }
    }
    std::cout << "\n";
}

void RunYear(size_t year) {
    auto fullTime = ScopedTimer("Year" + Constexpr::ToString(year) + " Time");
    for (const auto& [day, part] : GetSolutions()[year]) {
        RunOne(year, day);
    }
}

void RunAll() {
    auto fullTime = ScopedTimer("Total Time");
    for (const auto& [year, days] : GetSolutions()) {
        for (const auto& [day, parts] : days) {
            RunOne(year, day);
        }
        std::cout << "\n";
    }
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
    RunAll();
    //RunYear(2022);
    //RunOne(2022, 25);
    //RunLatest();

    PrintTimings(0, std::chrono::seconds(1));
    return 0;
}
