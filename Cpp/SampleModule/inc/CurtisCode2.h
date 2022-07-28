#pragma once
#include <limits>
#include <vector>


class Solution {
    constexpr static std::vector<int> GetRunLengths(const std::vector<int>& nums) {
        int currentDiff = std::numeric_limits<int>::max();
        int firstIndex = 0;
        int secondIndex = 1;
        int runLength = 0;
        std::vector<int> result{};

        for(; secondIndex < nums.size(); firstIndex++, secondIndex++) {
            int delta = nums[secondIndex] - nums[firstIndex];
            if(currentDiff == delta) {
                runLength++;
            } else {
                currentDiff = delta;
                result.push_back(runLength);
                runLength = 2;
            }
        }

        result.push_back(runLength);
        return result;
    }

    constexpr static int countSubSequences(int runLength) {
        return runLength < 3 ? 0 : ((runLength - 1) * (runLength - 2)) / 2;
    }

public:

    constexpr int CountSequencesLongerThan2(const std::vector<int>& nums) {
        if(nums.size() < 3)
            return 0;

        auto runs = GetRunLengths(nums);
        int result = 0;
        for(const auto& run: runs) {
            result += countSubSequences(run);
        }

        return result;
    }
};

Solution solution{};
static_assert(solution.CountSequencesLongerThan2({1, 2}) == 0);
static_assert(solution.CountSequencesLongerThan2({}) == 0);
static_assert(solution.CountSequencesLongerThan2({1, 2, 3}) == 1);
static_assert(solution.CountSequencesLongerThan2({1, 2, 3, 4}) == 3);
static_assert(solution.CountSequencesLongerThan2({2, 2, 3, 4, 4}) == 1);
static_assert(solution.CountSequencesLongerThan2({5,4,3,2,1}) == 6);
static_assert(solution.CountSequencesLongerThan2({2,4,6,8,10}) == 6);
static_assert(solution.CountSequencesLongerThan2({1,2,1,2,1,2}) == 0);