#pragma once
#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>

#define MY_CONCAT1(_a, _b) _a##_b
#define MY_CONCAT(_a, _b) MY_CONCAT1(_a, _b)

using namespace std;

bool myAssert(bool expr, string msg = "") {
    if(!expr) {
        cout << msg << endl;
    }
    return expr;
}
#define ENABLE_MY_ASSERTS

#ifdef ENABLE_MY_ASSERTS
#define MY_ASSERT(_expr, _msg) bool MY_CONCAT(assertResult, __LINE__) = myAssert(bool(_expr), "MY_ASSERT: " _msg "\n{" #_expr "}")
#else
#define MY_ASSERT(_expr, _msg)
#endif
#define MY_ASSERT_NO_MSG(_expr) MY_ASSERT(_expr, "")
MY_ASSERT(1 + 1 == 2, "This is just true...");
// MY_ASSERT(1+1 == 3, "This is just false...");



class Solution {

    void _forEachDifference(const vector<int>& nums, function<void(int)> reportDifference) const {
        MY_ASSERT(reportDifference, "_forEachDifference must be given a callback");

        for(size_t i = 0; i + 1 < nums.size(); ++i) {
            size_t iNext = i + 1;
            reportDifference(nums[iNext] - nums[i]);
        }
    }
    // Equivelance Classes:
    // - nums size too small
    // - num less than next num
    // - num equal to next num
    // - num greater than next num
    // - null reportDifference
    // - pairs are different
    // Boundaries:
    // - too small = less than a pair of nums (2)
    vector<int> _forEachDifference_test_getDiffs(const vector<int>& nums) {
        vector<int> diffs;
        _forEachDifference(nums, [&diffs, &nums](int diff) { diffs.push_back(diff); });
        return diffs;
    }

    MY_ASSERT(_forEachDifference_test_getDiffs({1}).empty(), "nums sizes under 1 should produce no diffs");
    MY_ASSERT(_forEachDifference_test_getDiffs({1, 2})[0] == 1, "lesser first number should produce a positive");
    MY_ASSERT(_forEachDifference_test_getDiffs({2, 1})[0] == -1, "greater first number should produce a negative");
    MY_ASSERT(_forEachDifference_test_getDiffs({1, 1})[0] == 0, "equal pair should produce a zero");
    MY_ASSERT(_forEachDifference_test_getDiffs({1, 1, 2}) == vector<int>({0, 1}), "differences should be different");

    void _forEachChangeInDifference(const vector<int>& nums, function<void(size_t)> reportNumberOfRepeatingDifferences) const {
        MY_ASSERT(reportNumberOfRepeatingDifferences, "_forEachChangeInDifference must be given a callback");

        if(nums.size() < 2) {
            return;
        }

        const int firstDiff = nums[1] - nums[0];

        int prevDiff = firstDiff;
        size_t prevDiffCount = 0;

        _forEachDifference(nums, [&prevDiff, &prevDiffCount, &reportNumberOfRepeatingDifferences](int currDiff) {
            if(currDiff == prevDiff) {
                prevDiffCount += 1;
            } else {
                reportNumberOfRepeatingDifferences(prevDiffCount);
                prevDiff = currDiff;
                prevDiffCount = 1;
            }
        });

        // report the final diff count
        reportNumberOfRepeatingDifferences(prevDiffCount);
    }

    // Equivelance Classes:
    // - nums size too small
    // - nums differences are unique
    // - nums differences all the same
    // - differences repeated later are counted as a different repeat
    // - null reportNumberOfRepeatingDifferences
    // Boundaries:
    // - too small = less than a pair of nums (2)
    vector<int> _forEachChangeInDifference_test_getDiffRepeats(const vector<int>& nums) {
        vector<int> diffRepeates;
        _forEachChangeInDifference(nums, [&diffRepeates, &nums](int diff) { diffRepeates.push_back(diff); });
        return diffRepeates;
    }

    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({}).empty(), "nums sizes under 1 should produce no diffs");
    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({1}).empty(), "nums sizes under 1 should produce no diffs");
    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({1, 2}) == vector<int>({1}), "a single difference has a repeat count of 1");
    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({1, 2, 1, 1}) == vector<int>({1, 1, 1}), "nums differences should be unique");
    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({1, 2, 3, 4}) == vector<int>({3}), "nums differences should be similar");
    MY_ASSERT(_forEachChangeInDifference_test_getDiffRepeats({1, 2, 3, 3, 4, 5}) == vector<int>({2, 1, 2}), "differences repeated later are counted as a different repeat");

    size_t _numberOfSubarraysInSubarraySize(size_t size) const {
        return ((size + 1) * size) / 2;
    }

    MY_ASSERT(_numberOfSubarraysInSubarraySize(0) == 0, "no elements means no subarray");
    MY_ASSERT(_numberOfSubarraysInSubarraySize(1) == 1, "one element means one subarray");
    MY_ASSERT(_numberOfSubarraysInSubarraySize(4) == 10, "4 elements means 10 subarrays");

public:
    int numberOfArithmeticSlices(const vector<int>& nums) {
        size_t totalSizeThreePlusArithmeticSequences = 0;

        _forEachChangeInDifference(nums, [this, &totalSizeThreePlusArithmeticSequences](size_t numberOfRepeatingDiffs) {
            const size_t localSliceSize = numberOfRepeatingDiffs;

            const size_t localSliceSubarrays = _numberOfSubarraysInSubarraySize(localSliceSize);
            const size_t localSliceSizeOneSubarrays = localSliceSize;

            const size_t localSliceSizeTwoPlusSubarrays = localSliceSubarrays - localSliceSizeOneSubarrays;
            const size_t localSliceSizeThreePlusArithmeticSequences = localSliceSizeTwoPlusSubarrays;

            totalSizeThreePlusArithmeticSequences += localSliceSizeThreePlusArithmeticSequences;
        });

        return totalSizeThreePlusArithmeticSequences;
    }
    MY_ASSERT(numberOfArithmeticSlices(vector<int>({1, 2})) == 0, "no arithmetic slices");
    MY_ASSERT(numberOfArithmeticSlices(vector<int>({1, 2, 3})) == 1, "a single arithmetic slice");
    MY_ASSERT(numberOfArithmeticSlices(vector<int>({1, 2, 3, 4})) == 3, "multiple arithmetic slice");
    MY_ASSERT(numberOfArithmeticSlices(vector<int>({2, 2, 3, 4, 4})) == 1, "a break in arithmetic slices");
};