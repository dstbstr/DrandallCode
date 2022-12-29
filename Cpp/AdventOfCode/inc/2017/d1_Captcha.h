#pragma once

#include "Common.h"

constexpr u32 SumDigits(std::string line, size_t offset) {
    u32 result = 0;
    for (auto i = 0; i < line.size(); i++) {
        if (line[i] == line[(i + offset) % line.size()]) {
            result += line[i] - '0';
        }
    }
    return result;
}

static_assert(SumDigits("1122", 1) == 3);
static_assert(SumDigits("1111", 1) == 4);
static_assert(SumDigits("1234", 1) == 0);
static_assert(SumDigits("91212129", 1) == 9);
static_assert(SumDigits("1212", 2) == 6);
static_assert(SumDigits("1221", 2) == 0);
static_assert(SumDigits("123425", 3) == 4);

auto PartOne(const std::vector<std::string>& lines) {
    return SumDigits(lines[0], 1);
}

auto PartTwo(const std::vector<std::string>& lines) {
    return SumDigits(lines[0], lines[0].size() / 2);
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}