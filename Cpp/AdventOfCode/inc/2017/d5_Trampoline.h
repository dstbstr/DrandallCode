#pragma once

#include "Common.h"

constexpr void Jump(u32& instruction, s32& distance) {
    instruction += distance;
    distance++;
}

constexpr void ShortJump(u32& instruction, s32& distance) {
    instruction += distance;
    if (distance > 2) {
        distance--;
    }
    else {
        distance++;
    }
}

template<typename F>
constexpr u32 StepsToExit(const std::vector<std::string> lines, F jumpFunc) {
    std::vector<s32> jumps;
    for (const auto& line : lines) {
        s32 jump;
        Constexpr::ParseNumber(line, jump);
        jumps.push_back(jump);
    }

    u32 steps = 0;
    u32 instruction = 0;
    while (instruction < jumps.size()) {
        jumpFunc(instruction, jumps[instruction]);
        steps++;
    }

    return steps;
}

static_assert(StepsToExit({ "0", "3", "0", "1", "-3" }, Jump) == 5);
static_assert(StepsToExit({ "0", "3", "0", "1", "-3" }, ShortJump) == 10);

auto PartOne(const std::vector<std::string>& lines) {
    return StepsToExit(lines, Jump);
}

auto PartTwo(const std::vector<std::string>& lines) {
    return StepsToExit(lines, ShortJump);
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}