#include "2020/d10_Jolts.h"

SOLUTION(2020, 10) {
    constexpr std::vector<size_t> ParseInput(const std::vector<std::string>&lines) {
        std::vector<size_t> nums;
        for (const auto& line : lines) {
            size_t num;
            Constexpr::ParseNumber(line, num);
            nums.push_back(num);
        }

        return nums;
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto nums = ParseInput(lines);
        nums.push_back(0);
        std::sort(nums.begin(), nums.end());
        size_t ones = 0;
        size_t threes = 1;
        for (size_t i = 0; i < nums.size() - 1; i++) {
            auto diff = nums[i + 1] - nums[i];
            if (diff == 1) ones++;
            else if (diff == 3) threes++;
        }

        return ones * threes;
    }

    constexpr size_t CountPaths(size_t runLength) {
        std::vector<size_t> known = { 1, 1, 2 };
        for (auto i = 0; i < 5; i++) {
            auto next = known[known.size() - 1] + known[known.size() - 2] + known[known.size() - 3];
            known.push_back(next);
        }

        return known[runLength];
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto nums = ParseInput(lines);
        nums.push_back(0);
        std::sort(nums.begin(), nums.end());
        size_t runLength = 0;
        size_t result = 1;
        for (auto i = 1; i < nums.size(); i++) {
            if (nums[i] - nums[i - 1] == 3) {
                result *= CountPaths(runLength);
                runLength = 0;
            }
            else {
                runLength++;
            }
        }
        result *= CountPaths(runLength);
        return result;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "16",
            "10",
            "15",
            "5",
            "1",
            "11",
            "7",
            "19",
            "6",
            "12",
            "4"
        };

        if (Part1(lines) != 35) return false;
        if (Part2(lines) != 8) return false;
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