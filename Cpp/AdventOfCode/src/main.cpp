#include "2015/SecretCode.h"

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

u64 RunAgainstInput() {
    //auto lines = ReadInputFile();
    return Run();
    //return 0;
}
/*
void Check() {
    if(RunTests()) {
        std::cout << "Tests Pass\n";
    } else {
        std::cout << "Tests Fail. :(\n";
    }
}
*/

int main(int, char**) {
    auto timer = ScopedTimer();

    //Check();
    std::cout << "Result: " << RunAgainstInput() << "\n";
    return 0;
}

