#pragma once

#include "Common.h"

u32 FindTopThreeElves(const std::vector<std::string>& lines) {
    std::vector<u32> elfCalories;
    u32 running = 0;
    for(const auto& line : lines) {
        if(line == "") {
            elfCalories.push_back(running);
            running = 0;
        } else {
            u32 num;
            ParseNumber(line, num);
            running += num;
        }
    }

    std::sort(elfCalories.begin(), elfCalories.end(), [](u32 lhs, u32 rhs) { return lhs > rhs;});
    return elfCalories[0] + elfCalories[1] + elfCalories[2];
}

//45ms
u32 FindMostCalories(const std::vector<std::string>& lines) {
    u32 best = 0;
    u32 running = 0;
    for(const auto& line : lines) {
        if(line == "") {
            best = std::max(best, running);
            running = 0;
        } else {
            u32 num;
            ParseNumber(line, num);
            running += num;
        }
    }

    return best;
}