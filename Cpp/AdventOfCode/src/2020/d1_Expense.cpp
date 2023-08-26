#include "2020/d1_Expense.h"

SOLUTION(2020, 1) {

    PART_ONE() {
        auto nums = ParseLinesAsNumbers<u32>(Lines);
        std::sort(nums.begin(), nums.end());
        for (auto firstNum = 0; firstNum < nums.size(); firstNum++) {
            bool tooBig = false;
            for (auto secondNum = firstNum + 1; secondNum < nums.size(); secondNum++) {
                auto sum = nums[firstNum] + nums[secondNum];
                if (sum == 2020) {
                    return Constexpr::ToString(nums[firstNum] * nums[secondNum]);
                }
                else if (sum > 2020) {
                    tooBig = true;
                    break;
                }
            }
            if (tooBig) continue;
        }

        return "Not Found";
    }

    PART_TWO() {
        auto nums = ParseLinesAsNumbers<u32>(Lines);
        std::sort(nums.begin(), nums.end());
        for (auto firstNum = 0; firstNum < nums.size(); firstNum++) {
            for (auto secondNum = firstNum + 1; secondNum < nums.size(); secondNum++) {
                bool tooBig = false;
                for (auto thirdNum = secondNum + 1; thirdNum < nums.size(); thirdNum++) {
                    auto sum = nums[firstNum] + nums[secondNum] + nums[thirdNum];
                    if (sum == 2020) {
                        return Constexpr::ToString(nums[firstNum] * nums[secondNum] * nums[thirdNum]);
                    }
                    else if (sum > 2020) {
                        tooBig = true;
                        break;
                    }
                }
                if (tooBig) continue;
            }
        }

        return "Not Found";
    }

    TESTS() {
        return true;
    }
}