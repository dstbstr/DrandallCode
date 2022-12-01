#pragma once

#include "Common.h"

#include <algorithm>

constexpr u32 CountPresents(u32 houseNumber) {
    auto divisors = GetDivisors(houseNumber);
    return SumConstexpr(divisors) * 10;
}

static_assert(CountPresents(1) == 10);
static_assert(CountPresents(2) == 30);
static_assert(CountPresents(786240) >= 34000000);

constexpr u32 CountLazyPresents(u32 houseNumber) {
    u32 threshold = houseNumber / 50;
    auto divisors = GetDivisors(houseNumber);
    std::vector<u32> keep;
    std::copy_if(divisors.begin(), divisors.end(), std::back_inserter(keep), [threshold](u32 divisor){
        return divisor > threshold;
    });

    return SumConstexpr(keep) * 11;
}

static_assert(CountLazyPresents(1) == 11);
static_assert(CountLazyPresents(2) == 33);

bool RunTests() {
    auto divisors = GetDivisors(24);
    std::sort(divisors.begin(), divisors.end());
    if(divisors.size() != 8) return false;
    if(divisors[0] != 1) return false;
    if(divisors[7] != 24) return false;

    return true;
}