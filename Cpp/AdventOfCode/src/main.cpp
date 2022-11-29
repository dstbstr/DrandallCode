#include "LightWar2.h"

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
    std::string input = StrUtil::JoinVec("", lines);
    auto arr = BuildArray<100, 100>(input);

    arr[0][0] = true;
    arr[0][99] = true;
    arr[99][0] = true;
    arr[99][99] = true;
    for(int i = 0; i < 100; i++) {
        //Next(arr);
        NextWithStuck(arr);
    }

    return CountLit(arr);
    //return 0;
}

void Check() {
    /*
    if(RunTests()) {
        std::cout << "Tests Pass\n";
    } else {
        std::cout << "Tests Fail. :(\n";
    }
    */
}

int main(int, char**) {
    auto timer = ScopedTimer();

    //Check();
    std::cout << "Result: " << RunAgainstInput() << "\n";
    return 0;
}

