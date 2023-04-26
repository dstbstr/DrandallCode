#pragma once

#include "Common.h"

constexpr std::vector<u32> ParseInput(const std::vector<std::string>& lines) {
    std::vector<u32> result;
    for (const auto& line : lines) {
        u32 num;
        Constexpr::ParseNumber(line, num);
        result.push_back(num);
    }

    return result;
}

constexpr size_t CountIncreasing(const std::vector<u32>& nums) {
    size_t result = 0;
    for (auto i = 1; i < nums.size(); i++) {
        result += nums[i] > nums[i - 1];
    }

    return result;
}

static_assert(CountIncreasing({ 199, 200, 208, 210, 200, 207, 240, 269, 260, 263 }) == 7);

constexpr auto PartOne(const std::vector<std::string>& lines) {
    return CountIncreasing(ParseInput(lines));
}

constexpr size_t CountIncreasingWindow(const std::vector<u32>& nums) {
    size_t result = 0;
    for (auto i = 0; i < nums.size() - 3; i++) {
        auto lhs = nums[i] + nums[i + 1] + nums[i + 2];
        auto rhs = lhs - nums[i] + nums[i + 3];
        result += lhs < rhs;
    }
    return result;
}

static_assert(CountIncreasingWindow({ 199, 200, 208, 210, 200, 207, 240, 269, 260, 263 }) == 5);

constexpr auto PartTwo(const std::vector<std::string>& lines) {
    return CountIncreasingWindow(ParseInput(lines));
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}