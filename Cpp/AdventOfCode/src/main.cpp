#include "PerfectCookie.h"

#include "Utilities/StringUtils.h"
#include "Utilities/ScopedTimer.h"
#include "Instrumentation/LogWriter/StdOutLogWriter.h"

#include <iostream>
#include <charconv>
#include <fstream>
#include <array>

Log::StdOutLogWriter logWriter{};

std::vector<std::string> ReadInputFile() {
    std::vector<std::string> result{};

    auto stream = std::ifstream("AdventOfCode\\input.txt");
    if(!stream.is_open()) {
        return {};
    }

    std::string line;
    while(std::getline(stream, line)) {
        result.push_back(StrUtil::Trim(line));
    }

    return result;
}

u32 RunAgainstInput() {
    auto lines = ReadInputFile();
    std::vector<Ing> ings;
    for(const auto& line : lines) {
        ings.push_back(ParseIng(line));
    }

    return FindBestScore(ings);
   //return 0;
}

void Check() {
    if(RunTests()) {
        std::cout << "Tests Pass\n";
    } else {
        std::cout << "Tests Fail. :(\n";
    }
}

int main(int, char**) {
    auto timer = ScopedTimer();

    //Check();
    std::cout << "Result: " << RunAgainstInput() << "\n";
    return 0;
}

