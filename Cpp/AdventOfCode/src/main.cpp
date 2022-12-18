#include "2022/Pressure.h"
//#include "2016/Scrambler.h"

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

    auto stream = std::ifstream("C:\\Users\\durandal\\source\\repos\\Scratch\\AdventOfCode\\AdventOfCode\\input.txt");
    if(!stream.is_open()) {
        return {};
    }

    std::string line;
    while(std::getline(stream, line)) {
        result.push_back(StrUtil::Trim(line));
    }

    return result;
}

void RunAgainstInput() {
    auto lines = ReadInputFile();
    std::cout << Run(lines) << "\n";
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
    RunAgainstInput();
    return 0;
}
