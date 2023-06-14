#pragma once

#include "2015/d5_NiceString.h"

SOLUTION(2015, 5) {
    static constexpr auto naughtyPairs = { "ab", "cd", "pq", "xy" };

    constexpr bool IsVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    constexpr bool IsNice(const std::string& input) {
        for (const auto& badPair : naughtyPairs) {
            if (input.find(badPair) != input.npos) return false;
        }

        auto vowelCount = 0;
        bool hasDouble = false;
        for (size_t i = 0; i < input.size(); i++) {
            vowelCount += IsVowel(input[i]);
            hasDouble |= (i > 0 && input[i] == input[i - 1]);
        }
        return vowelCount > 2 && hasDouble;
    }

    constexpr std::string ExtractPair(const std::string& str, size_t firstChar) {
        std::string pair;
        pair.push_back(str[firstChar]);
        pair.push_back(str[firstChar + 1]);
        return pair;
    }
    constexpr bool IsReallyNice(const std::string& input) {
        bool hasSplit = false;
        for (size_t i = 2; i < input.size(); i++) {
            if (input[i - 2] == input[i]) {
                hasSplit = true;
                break;
            }
        }
        if (!hasSplit) return false;

        std::array<size_t, Constexpr::FromBase26("zz") + 1> seen{};
        for (size_t i = 1; i < input.size(); i++) {
            auto pair = ExtractPair(input, i - 1);
            if (i > 1 && ExtractPair(input, i - 2) == pair) continue;
            seen[Constexpr::FromBase26(pair)]++;
        }

        return Constexpr::FindMax(seen) > 1;
    }

    PART_ONE() {
        return Constexpr::ToString(std::count_if(lines.cbegin(), lines.cend(), IsNice));
    }

    PART_TWO() {
        return Constexpr::ToString(std::count_if(lines.cbegin(), lines.cend(), IsReallyNice));
    }

    TESTS() {
        static_assert(IsNice("ugknbfddgicrmopn"));
        static_assert(IsNice("aaa"));

        static_assert(!IsNice("jchzalrnumimnmhp"));
        static_assert(!IsNice("haegwjzuvuyypxyu"));
        static_assert(!IsNice("dvszwmarrgswjxmb"));

        static_assert(IsReallyNice("qjhvhtzxzqqjkmpb"));
        static_assert(IsReallyNice("xxyxx"));

        static_assert(!IsReallyNice("uurcxstgmygtbstg"));
        static_assert(!IsReallyNice("ieodomkazucvgmuy"));
        static_assert(!IsReallyNice("aaa"));

        return true;
    }
}