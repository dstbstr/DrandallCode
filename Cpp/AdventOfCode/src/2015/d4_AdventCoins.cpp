#pragma once

#include "2015/d4_AdventCoins.h"
#include "Algorithms/md5Constexpr.h"

SOLUTION(2015, 4) {
    constexpr size_t Solve(std::string_view input, const std::string& prefix, size_t hint = 0) {
        size_t result = hint;
        auto asString = std::string(input);
        while (true) {
            auto key = asString + Constexpr::ToString(result);
            auto hash = ToHexUpper(md5::compute(key.c_str()));
            if (hash.substr(0, prefix.size()) == prefix) break;
            result++;
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Line, "00000", 100'000));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Line, "000000", 3'900'000));
    }

    TESTS() {
        if (Solve("abcdef", "00000", 600000) != 609043) return false;
        if (Solve("pqrstuv", "00000", 1000000) != 1048970) return false;

        return true;
    }
}