#include "2020/d1_Expense.h"

SOLUTION(2020, 1) {
    constexpr std::vector<u32> ParseInput(const std::vector<std::string>&lines) {
        std::vector<u32> result;
        for (const auto& line : lines) {
            u32 num;
            Constexpr::ParseNumber(line, num);
            result.push_back(num);
        }
        return result;
    }

    auto Part1(const std::vector<std::string>&lines) {
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

    auto Part2(const std::vector<std::string>&lines) {
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

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {

        };

        if (Part1(lines) != 0) return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}