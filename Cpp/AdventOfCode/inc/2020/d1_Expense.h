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

auto PartOne(const std::vector<std::string>& lines) {
    auto nums = ParseInput(lines);
    std::sort(nums.begin(), nums.end());
    for (auto firstNum = 0; firstNum < nums.size(); firstNum++) {
        bool tooBig = false;
        for (auto secondNum = firstNum + 1; secondNum < nums.size(); secondNum++) {
            auto sum = nums[firstNum] + nums[secondNum];
            if (sum == 2020) {
                return nums[firstNum] * nums[secondNum];
            }
            else if (sum > 2020) {
                tooBig = true;
                break;
            }
        }
        if (tooBig) continue;
    }
    
    return 0u;
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto nums = ParseInput(lines);
    std::sort(nums.begin(), nums.end());
    for (auto firstNum = 0; firstNum < nums.size(); firstNum++) {
        for (auto secondNum = firstNum + 1; secondNum < nums.size(); secondNum++) {
            bool tooBig = false;
            for (auto thirdNum = secondNum + 1; thirdNum < nums.size(); thirdNum++) {
                auto sum = nums[firstNum] + nums[secondNum] + nums[thirdNum];
                if (sum == 2020) {
                    return nums[firstNum] * nums[secondNum] * nums[thirdNum];
                }
                else if (sum > 2020) {
                    tooBig = true;
                    break;
                }
            }
            if (tooBig) continue;
        }
    }
    return 0u;
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