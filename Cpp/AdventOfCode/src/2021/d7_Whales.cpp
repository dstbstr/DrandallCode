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

    auto Part1(const std::string & line) {
        auto nums = ParseLineAsNumbers<u32>(line);
        std::sort(nums.begin(), nums.end());
        auto target = FindTarget(nums);
        size_t result = 0;
        for (auto num : nums) {
            result += Constexpr::AbsDistance(num, target);
        }
        return result;
    }

    auto Part2(const std::string & line) {
        auto nums = ParseLineAsNumbers<u32>(line);
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

        return std::min(result1, result2);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        std::string line = "16,1,2,0,4,2,7,1,2,14";
        if (Part1(line) != 37) return false;
        if (Part2(line) != 168) return false;
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