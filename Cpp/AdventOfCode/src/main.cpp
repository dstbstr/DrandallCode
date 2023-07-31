#include "All.h"

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

        if (!GET_LOGS().empty()) {
            std::cout << "## Logs ##\n";
            std::cout << Constexpr::JoinVec("\n", GET_LOGS());
            GET_LOGS().clear();
        }
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

//maybe look closer at this for more shenanigans: https://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char

int main(int, char**) {
    Constexpr::ConstexprMathTests::RunTests();
    //RunAll();
    //RunYear(2020);
    RunOne(2020, 11);
    //RunLatest();
    return 0;
}
