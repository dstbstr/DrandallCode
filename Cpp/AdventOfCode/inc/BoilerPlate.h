#pragma once

#include "Common.h"

auto PartOne(const std::vector<std::string>& lines) {
    return lines.size();
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(PartOne(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {

    };

    if (PartOne(lines) != 0) return false;
    return true;
}