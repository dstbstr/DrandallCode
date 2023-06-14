#include "2015/All.h"

#include "Common.h"
#include "Utilities/StringUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"

#include <iostream>
#include <charconv>
#include <fstream>
#include <array>
#include <filesystem>

Log::MinimalStdOutLogWriter logWriter{};

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

/*
void RunAgainstInput() {
    auto lines = ReadInputFile(2015, 1);
    std::cout << Run(lines) << "\n";
}

void Check() {
    if(RunTests()) {
        std::cout << "Tests Pass\n";
    } else {
        std::cout << "Tests Fail. :(\n";
    }
}
*/

bool Check(size_t year, size_t day) {
    if (!GetTests().contains(year) || !GetTests()[year].contains(day)) {
        return true;
    }

    if (auto testTime = ScopedTimer("Test Time"); GetTests()[year][day]()) {
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
        auto partTime = ScopedTimer();
        std::cout << "Part " << part << ": " << func(lines) << "\n";
    }
    std::cout << "\n";
}

void RunYear(size_t year) {
    auto fullTime = ScopedTimer("Total Time");
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

void RunLatest() {
    auto& solutions = GetSolutions();
    size_t maxYear = 0;
    for (const auto& [year, days] : solutions) {
        maxYear = std::max(maxYear, year);
    }
    size_t maxDay = 0;
    for (const auto& [day, parts] : solutions[maxYear]) {
        maxDay = std::max(maxDay, day);
    }

    RunOne(maxYear, maxDay);
}

int main(int, char**) {
    //RunAll();
    RunYear(2015);
    //RunOne(2015, 25);
    //RunLatest();
    return 0;
}
