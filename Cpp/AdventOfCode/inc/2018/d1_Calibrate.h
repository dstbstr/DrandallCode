#pragma once

#include "Common.h"

constexpr std::vector<s32> ParseDeltas(const std::vector<std::string>& lines) {
    std::vector<s32> result;
    for (const auto& line : lines) {
        s32 number;
        if (line[0] == '+') {
            Constexpr::ParseNumber(line.substr(1), number);
        }
        else {
            Constexpr::ParseNumber(line, number);
        }
        result.push_back(number);
    }

    return result;
}

auto PartOne(const std::vector<std::string>& lines) {
    auto deltas = ParseDeltas(lines);
    return Constexpr::Sum(deltas);
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto deltas = ParseDeltas(lines);
    std::unordered_set<s32> seen;
    s32 frequency = 0;
    size_t currentDelta = 0;

    while (true) {
        if (seen.find(frequency) != seen.end()) {
            return frequency;
        }
        seen.insert(frequency);
        frequency += deltas[currentDelta++];
        if (currentDelta == deltas.size()) {
            currentDelta = 0;
        }
    }
    
    return 0;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {

    };

    if (PartOne(lines) != 0) return false;
    return true;
}