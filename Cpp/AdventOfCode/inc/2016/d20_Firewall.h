#pragma once

#include "Common.h"

std::vector<UCoord> GetRanges(const std::vector<std::string>& lines) {
    std::vector<UCoord> ranges;
    for (const auto& line : lines) {
        auto split = StrUtil::Split(line, "-");
        UCoord range;
        ParseNumber(split[0], range.X);
        ParseNumber(split[1], range.Y);
        ranges.push_back(range);
    }

    return ranges;
}

u32 FindLowest(const std::vector<std::string>& lines) {
    auto ranges = GetRanges(lines);

    //std::sort(ranges.cbegin(), ranges.cend(), &UCoord::X);
    std::ranges::sort(ranges, {}, &UCoord::X);

    auto bottomRange = ranges[0];
    for (size_t i = 1; i < ranges.size(); i++) {
        auto next = ranges[i];
        if (bottomRange.Y >= next.X - 1) {
            bottomRange.Y = std::max(bottomRange.Y, next.Y);
        }
        else {
            return bottomRange.Y + 1;
        }
    }

    return 0;
}

u32 CountAvailable(const std::vector<std::string>& lines, u32 max) {
    auto ranges = GetRanges(lines);
    std::ranges::sort(ranges, {}, &UCoord::X);

    u32 result = 0;
    auto current = ranges[0];
    for (size_t i = 1; i < ranges.size(); i++) {
        auto next = ranges[i];
        if (current.Y >= next.X - 1) {
            current.Y = std::max(current.Y, next.Y);
        }
        else {
            result += next.X - current.Y - 1;
            current.Y = next.Y;
        }
    }

    if (current.Y < max) {
        result += max - current.Y;
    }

    return result;
}


std::string Run(const std::vector<std::string>& lines) {
    //return ToString(FindLowest(lines));
    return ToString(CountAvailable(lines, 4294967295));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "5-8",
        "0-2",
        "4-7"
    };

    if (FindLowest(lines) != 3) return false;
    if (CountAvailable(lines, 9) != 2) return false;

    return true;
}