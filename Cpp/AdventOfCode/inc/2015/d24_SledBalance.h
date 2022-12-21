#pragma once

#include "Common.h"
#include <iostream>

//All weights equal
//Then minimize compartment 1 count
//Then minimze compartment 1 product

//1 + 2 + 3 + 4 + 5 + 7 + 8 + 9 + 10 + 11 = 60
//60 / 3 = 20
//11 + 9 = 20
//if compartment 1 adds up to 20, the others will too
//start with greatest weight, and work down

constexpr u32 FindBalanceWeight(std::vector<u32> weights, u32 spots) {
    return SumConstexpr(weights) / spots;
}

static_assert(FindBalanceWeight({1, 2, 3, 4, 5, 7, 8, 9, 10, 11}, 3) == 20);
static_assert(FindBalanceWeight({1, 2, 3, 4, 5, 7, 8, 9, 10, 11}, 4) == 15);

constexpr u64 FindEntanglement(std::vector<u32> weights) {
    u64 result =1;
    for(auto weight: weights) {
        result *= weight;
    }
    return result;
}

static_assert(FindEntanglement({11,9}) == 99);
static_assert(FindEntanglement({10,9,1}) == 90);

//static size_t bestSizeSoFar = 999;

constexpr bool Recurse(const std::vector<u32>& weights, u32 index, u32 remainingWeight, std::vector<u32>& outResult, u32 initialWeight) {
    if(remainingWeight == 0) {
        return true;
    }
    if(outResult.size() > 6) {
        return false;
    }

    u32 minItems = 999;
    u64 minEntanglement = 99999;
    bool found = false;
    std::vector<u32> result;
    for(auto i = index; i < weights.size(); i++) {
        std::vector<u32> temp = outResult;
        auto weight = weights[i];
        if(weight > remainingWeight) {
            continue;
        }
        temp.push_back(weight);
        if(Recurse(weights, i + 1, remainingWeight - weight, temp, initialWeight)) {
            found = true;
            auto entanglement = FindEntanglement(temp);
            if(temp.size() < minItems) {
                minItems = static_cast<u32>(temp.size());
                minEntanglement = entanglement;
                result = temp;
            } else if(temp.size() == minItems) {
                if(entanglement < minEntanglement) {
                    minEntanglement = entanglement;
                    result = temp;
                }
            }
        }
    }

    if(found) {
        outResult = result;
    }
    return found;
}

constexpr u64 FindMinimalEntanglement(std::vector<u32> weights, u32 spots) {
    std::vector<u32> best;
    auto balance = FindBalanceWeight(weights, spots);
    Recurse(weights, 0, balance, best, balance);

    return FindEntanglement(best);
}

bool RunTests() {
    return true;
}