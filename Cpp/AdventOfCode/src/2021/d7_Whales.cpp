#include "2021/d7_Whales.h"

SOLUTION(2021, 7) {
    constexpr u32 FindTarget(const std::vector<u32>&nums) {
        if (nums.size() % 2 == 0) {
            return (nums[nums.size() / 2] + nums[(nums.size() / 2) - 1]) / 2;
        }
        else {
            return nums[nums.size() / 2];
        }
    }

    PART_ONE() {
        auto nums = ParseLineAsNumbers<u32>(lines[0]);
        std::sort(nums.begin(), nums.end());
        auto target = FindTarget(nums);
        return Constexpr::ToString(std::accumulate(nums.begin(), nums.end(), 0ull, [&](size_t previous, u32 num) {
            return previous + Constexpr::AbsDistance(num, target);
            }));
    }

    PART_TWO() {
        auto nums = ParseLineAsNumbers<u32>(lines[0]);
        u32 total = std::accumulate(nums.begin(), nums.end(), 0);
        u32 average = static_cast<u32>(total / nums.size());

        size_t result1 = 0;
        size_t result2 = 0;
        for (auto num : nums) {
            auto distance = Constexpr::AbsDistance(num, average);
            result1 += (distance * (distance + 1)) / 2;

            auto d2 = Constexpr::AbsDistance(num, average + 1);
            result2 += (d2 * (d2 + 1)) / 2;
        }

        return Constexpr::ToString(std::min(result1, result2));
    }

    TESTS() {
        std::vector<std::string> lines = { "16,1,2,0,4,2,7,1,2,14" };
        if (PartOne(lines) != "37") return false;
        if (PartTwo(lines) != "168") return false;
        return true;
    }
}