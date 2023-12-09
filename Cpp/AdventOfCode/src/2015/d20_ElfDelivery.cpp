#pragma once

#include "2015/d20_ElfDelivery.h"
SOLUTION(2015, 20) {
    constexpr u32 CountPresents(u32 houseNumber) {
        auto divisors = Constexpr::GetDivisors(houseNumber);
        return std::accumulate(divisors.cbegin(), divisors.cend(), 0) * 10;
    }

    constexpr u32 CountLazyPresents(u32 houseNumber) {
        u32 threshold = houseNumber / 50;
        auto divisors = Constexpr::GetDivisors(houseNumber);
        std::vector<u32> keep;
        std::copy_if(divisors.begin(), divisors.end(), std::back_inserter(keep), [threshold](u32 divisor) {
            return divisor > threshold;
            });

        return std::accumulate(keep.cbegin(), keep.cend(), 0) * 11;
    }

    constexpr u32 Solve(std::string_view line, auto func, u32 hint = 0) {
        u32 target;
        Constexpr::ParseNumber(line, target);
        u32 current = hint;
        while (func(current) < target) {
            current++;
        }
        return current;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines[0], CountPresents, 700'000));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines[0], CountLazyPresents, 800'000));
    }
    TESTS() {
        static_assert(CountPresents(1) == 10);
        static_assert(CountPresents(2) == 30);
        static_assert(CountPresents(786240) >= 34'000'000);

        static_assert(CountLazyPresents(1) == 11);
        static_assert(CountLazyPresents(2) == 33);

        auto divisors = Constexpr::GetDivisors(24);
        std::sort(divisors.begin(), divisors.end());
        if (divisors.size() != 8) return false;
        if (divisors[0] != 1) return false;
        if (divisors[7] != 24) return false;

        return true;
    }
}