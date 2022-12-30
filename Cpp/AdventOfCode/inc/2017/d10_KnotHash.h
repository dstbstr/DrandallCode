#pragma once

#include "Common.h"
#include "KnotHash.h"

auto PartOne(const std::vector<u32>& lengths) {
    auto numbers = GetInitialNumbers();

    u32 skip = 0;
    u32 currentPos = 0;

    for (auto length : lengths) {
        Twist(numbers, length, skip, currentPos);
    }

    std::rotate(numbers.rbegin(), numbers.rbegin() + currentPos, numbers.rend());
    return numbers[0] * numbers[1];
}

auto PartTwo(const std::string& line) {
    return HashToHex(KnotHash::Hash(line));
}

std::string Run(const std::vector<std::string>&) {
    std::vector<u32> input = { 212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164 };

    //return ToString(PartOne(input));
    return ToString(PartTwo("212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164"));
}

bool RunTests() {
    if (PartTwo("1,2,3") != "3efbe78a8d82f29979031a4aa0b16a9d") return false;
    return true;
}