#pragma once

#include "Common.h"

constexpr bool Matches(u32 val) {
    auto str = Constexpr::ToString(val);

    bool hasPair = false;
    for (auto i = 1; i < str.size(); i++) {
        if (str[i] < str[i - 1]) return false;
        if (str[i] == str[i - 1]) hasPair = true;
    }

    return hasPair;
}

static_assert(Matches(111111));
static_assert(!Matches(223450));
static_assert(!Matches(123456));

constexpr bool ExtendedMatch(u32 val) {
    auto str = Constexpr::ToString(val);
    std::vector<std::string> pairs;

    for (auto i = 0; i < str.size() - 1; i++) {
        if (str[i + 1] < str[i]) return false;
        if (str[i] == str[i + 1]) {
            pairs.push_back(str.substr(i, 2));
        }
    }

    for (const auto& pair : pairs) {
        if (str.find(pair) + 1 == str.find_last_of(pair)) return true;
    }
    return false;
}


static_assert(ExtendedMatch(112233));
static_assert(!ExtendedMatch(123444));
static_assert(ExtendedMatch(111122));


constexpr auto PartOne(u32 begin, u32 end) {
    u32 result = 0;
    u32 current = begin;
    while (current <= end) {
        result += Matches(current++);
    }

    return result;
}

constexpr auto PartTwo() {
    u32 result = 0;
    u32 current = 152085;
    while (current <= 670283) {
        result += ExtendedMatch(current++);
    }

    return result;
}

std::string Run(const std::vector<std::string>&) {
    //return ToString(PartOne(152085, 670283));
    return Constexpr::ToString(PartTwo());
}

bool RunTests() {
    if(!ExtendedMatch(112233)) return false;
    if(ExtendedMatch(123444)) return false;
    if(!ExtendedMatch(111122)) return false;
    return true;
}