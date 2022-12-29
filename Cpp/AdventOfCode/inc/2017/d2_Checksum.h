#pragma once

#include "Common.h"

constexpr u32 FindDelta(const std::string& line) {
    u32 min = 99999;
    u32 max = 0;
    auto split = Constexpr::Split(line, "\t");
    for (auto str : split) {
        u32 val;
        Constexpr::ParseNumber(str, val);
        min = std::min(min, val);
        max = std::max(max, val);
    }

    return max - min;
}

static_assert(FindDelta("5\t1\t9\t5") == 8);
static_assert(FindDelta("7\t5\t3") == 4);
static_assert(FindDelta("2\t4\t6\t8") == 6);

constexpr u32 FindDivisors(const std::string& line) {
    auto split = Constexpr::Split(line, "\t");
    std::vector<u32> values;
    for (auto str : split) {
        u32 val;
        Constexpr::ParseNumber(str, val);
        values.push_back(val);
    }

    for (auto val : values) {
        for (auto other : values) {
            if (val <= other) continue;
            if (val % other == 0) {
                return val / other;
            }
        }
    }

    return 0;
}

template<typename F>
constexpr u32 Checksum(const std::vector<std::string>& lines, F func) {
    u32 result = 0;
    for (const auto& line : lines) {
        result += func(line);
    }
    return result;
}
static_assert(Checksum(std::vector<std::string>{"5\t1\t9\t5", "7\t5\t3", "2\t4\t6\t8"}, FindDelta) == 18);
static_assert(Checksum(std::vector<std::string>{"5\t9\t2\t8", "9\t4\t7\t3", "3\t8\t6\t5"}, FindDivisors) == 9);

auto PartOne(const std::vector<std::string>& lines) {
    return Checksum(lines, FindDelta);
}

auto PartTwo(const std::vector<std::string>& lines) {
    return Checksum(lines, FindDivisors);
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}