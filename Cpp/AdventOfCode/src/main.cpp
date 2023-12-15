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

//yuck.  Needed to find the exe path
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <libloaderapi.h>

Log::MinimalStdOutLogWriter logWriter{};
using TimingEntry = std::pair<std::string, std::chrono::microseconds>;

enum struct Visibility { Hide, Show };

std::vector<std::string> ReadInputFile(size_t year, size_t day) {
    std::vector<std::string> result{};
    auto buffer = std::make_unique<char*>(new char[1024]);
    GetModuleFileNameA(nullptr, *buffer, MAX_PATH);
    auto exePath = std::string(*buffer);
    exePath = exePath.substr(0, exePath.find_last_of("/\\"));
    auto targetFile = std::filesystem::path(exePath) / "Inputs" / ToString(year) / ("d" + ToString(day) + ".txt");
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

bool Check(size_t year, size_t day, std::vector<TimingEntry>& timingData, Visibility visibility = Visibility::Show) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }

    auto GatherTiming = [&](std::chrono::microseconds elapsed) {
        auto key = Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":Test";
        timingData.push_back(std::make_pair(key, elapsed));
        };

    if (auto testTime = ScopedTimer(GatherTiming, "Test Time"); GetTests()[year][day]()) {
        if(visibility == Visibility::Show) std::cout << "Test Pass\n";
        return true;
    }
    else {
        if(visibility == Visibility::Show) std::cout << "Test Fail. :(\n";
        return false;
    }
}

std::vector<TimingEntry> RunOne(size_t year, size_t day, Visibility visibility = Visibility::Show) {
    if (visibility == Visibility::Show) std::cout << "### " << year << " Day " << day << "###\n";
    std::vector<TimingEntry> timingData;

    if (!Check(year, day, timingData, visibility)) return timingData;

    for (const auto& [part, func] : GetSolutions()[year][day]) {
        auto GatherTiming = [&](std::chrono::microseconds elapsed) {
            auto key = Constexpr::ToString(year) + "/" + Constexpr::ToString(day) + ":" + Constexpr::ToString(part);
            timingData.push_back(std::make_pair(key, elapsed));
            };
        std::string result;
        auto input = ReadInputFile(year, day);
        {
            auto partTime = ScopedTimer(GatherTiming);
            result = func(input);
        }
        if (visibility == Visibility::Show) {
            std::cout << "Part " << part << ": " << result << "\n";

            if (!GET_LOGS().empty()) {
                std::cout << "## Logs ##\n";
                std::cout << Constexpr::JoinVec("\n", GET_LOGS());
                GET_LOGS().clear();
            }
        }
    }
    if (visibility == Visibility::Show) {
        std::cout << "\n";
    }

    return timingData;
}

std::vector<TimingEntry> RunTasks(const TaskQueue<std::vector<TimingEntry>>& tasks) {
    auto timingData = Runner::Get().RunAll(tasks);
    std::vector<TimingEntry> result;
    for (const auto& d : timingData) {
        std::copy(d.begin(), d.end(), std::back_inserter(result));
    }
    return result;
}
std::vector<TimingEntry> RunYear(size_t year) {
    auto tasks = TaskQueue<std::vector<TimingEntry>>{};
    for (const auto& [day, part] : GetSolutions()[year]) {
        tasks.push([=]() { return RunOne(year, day, Visibility::Hide); });
    }
    return RunTasks(tasks);
}

std::vector<TimingEntry> RunYearSync(size_t year) {
    std::vector<TimingEntry> result;
    for (const auto& [day, parts] : GetSolutions()[year]) {
        auto timings = RunOne(year, day, Visibility::Show);
        std::copy(timings.begin(), timings.end(), std::back_inserter(result));
    }
    return result;
}

std::vector<TimingEntry> RunAll() {
    auto tasks = TaskQueue<std::vector<TimingEntry>>{};
    for (const auto& [year, days] : GetSolutions()) {
        for (const auto& [day, parts] : days) {
            tasks.push([=]() { return RunOne(year, day, Visibility::Hide); });
        }
    }
    return RunTasks(tasks);
}

std::vector<TimingEntry> RunAllSync() {
    std::vector<TimingEntry> result;
    for (const auto& [year, days] : GetSolutions()) {
        for (const auto& [day, parts] : days) {
            auto timings = RunOne(year, day, Visibility::Show);
            std::copy(timings.begin(), timings.end(), std::back_inserter(result));
        }
    }

    return result;
}

void PrintTimings(std::vector<TimingEntry> timings, size_t maxResults = 0, std::chrono::microseconds minElapsed = std::chrono::microseconds(0)) {
    std::sort(timings.begin(), timings.end(), [](auto lhs, auto rhs) { return rhs.second < lhs.second; });

    size_t resultId = 0;
    for (auto [key, elapsed] : timings) {
        if (minElapsed > std::chrono::microseconds(0) && elapsed < minElapsed) break;
        if (maxResults > 0 && resultId > maxResults) break;

        std::cout << key << ": " << TimeUtils::DurationToString(elapsed) << "\n";
    }
}

void RunFromCommandLine(int argc, char** argv) {
    std::vector<TimingEntry> timings;
    if (argv[1][0] == '*') {
        timings = RunAll();
    }
    else {
        size_t year;
        Constexpr::ParseNumber(argv[1], year);
        if (argc > 2) {
            if (argv[2][0] == '*') {
                //timings = RunYearSync(year);
                timings = RunYear(year);
            }
            else {
                size_t day;
                Constexpr::ParseNumber(argv[2], day);
                timings = RunOne(year, day);
            }
        }
        else {
            //timings = RunYearSync(year);
            timings = RunYear(year);
        }
    }
    PrintTimings(timings);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        RunFromCommandLine(argc, argv);
        return 0;
    }


    //auto timings = RunAllSync();
    //auto timings = RunAll();
    //auto timings = RunYearSync(2023);
    //auto timings = RunYear(2023);
    auto timings = RunOne(2023, 16);

    //PrintTimings(0, std::chrono::seconds(1));
    PrintTimings(timings);
    return 0;
}
