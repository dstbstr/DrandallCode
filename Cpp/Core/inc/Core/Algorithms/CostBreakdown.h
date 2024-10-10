#pragma once

#include "Common.h"

namespace _impl {
    using Cache = Constexpr::SmallMap<size_t, Constexpr::BigSet<int>>;

    constexpr void rec(Cache& cache, const std::vector<int>& nums, size_t numIdx) {
        if (numIdx >= nums.size()) {
            return;
        }

        Constexpr::BigSet<int> result{ nums[numIdx] };

        if (!cache.contains(numIdx + 1)) {
            rec(cache, nums, numIdx + 1);
        }
        for (auto val : cache[numIdx + 1].GetValues()) {
			result.insert(val);
			result.insert(val + nums[numIdx]);
		}

        cache[numIdx] = result;
    }

    constexpr std::pair<std::vector<int>, std::vector<int>> RecreatePath(const Cache & cache, const std::vector<int>& nums, int target) {
        auto original = target;
        std::vector<int> p1{};
        std::vector<int> p2{};

        for (size_t numIdx = 0; numIdx < nums.size(); numIdx++) {
            auto val = nums[numIdx];
            if (cache.at(numIdx + 1).contains(target)) {
                p1.push_back(val);
            }
            else {
                p2.push_back(val);
                target -= val;
            }

            if (target == 0) {
                while (++numIdx < nums.size()) {
                    p1.push_back(nums[numIdx]);
                }
                break;
            }
        }

        auto sum = std::accumulate(p2.begin(), p2.end(), 0, std::plus<int>());
        if (target != 0 || sum != original) {
            throw "Fail";
        }

        return { p1, p2 };
    }

}
constexpr std::pair<std::vector<int>, std::vector<int>> SplitExpensesIntoGroups(const std::vector<int>& expenses, int targetTotal) {
    _impl::Cache cache{};
    _impl::rec(cache, expenses, 0);
    return _impl::RecreatePath(cache, expenses, targetTotal);
}