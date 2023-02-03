#pragma once

#include "Common.h"

constexpr u64 Calculate(u64 mass) {
    auto quotient = mass / 3;
    if (quotient < 2) return 0;
    return quotient - 2;
}

constexpr u64 CalculateWithFuel(u64 mass) {
    u64 result = 0;
    auto delta = Calculate(mass);
    while (delta > 0) {
        result += delta;
        delta = Calculate(delta);
    }

    return result;
}

static_assert(Calculate(12) == 2);
static_assert(Calculate(14) == 2);
static_assert(Calculate(1969) == 654);
static_assert(Calculate(100756) == 33583);

static_assert(CalculateWithFuel(1969) == 966);
static_assert(CalculateWithFuel(100756) == 50346);

constexpr std::vector<u64> GetMasses(const std::vector<std::string>& lines) {
    std::vector<u64> result;
    for (const auto& line : lines) {
        u64 mass;
        Constexpr::ParseNumber(line, mass);
        result.push_back(mass);
    }

    return result;
}

constexpr auto PartOne(const std::vector<std::string>& lines) {
    u64 result = 0;
    auto masses = GetMasses(lines);
    for (auto mass : masses) {
        result += Calculate(mass);
    }

    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    u64 result = 0;
    auto masses = GetMasses(lines);
    for (auto mass : masses) {
        result += CalculateWithFuel(mass);
    }

    return result;
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