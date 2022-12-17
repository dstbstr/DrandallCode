#pragma once

#include "Common.h"

u32 FindLeftWinner(u32 playerCount) {
    auto power = static_cast<u32>(std::floor(std::log2(playerCount)));
    auto remainder = playerCount - std::pow(2, power);
    return static_cast<u32>(2 * remainder + 1);
}

u32 FindAcrossWinner(u32 playerCount) {
    auto pow = static_cast<u32>(std::floor(std::log(playerCount) / std::log(3)));
    auto b = static_cast<u32>(std::pow(3, pow));
    if (playerCount == b) return playerCount;
    if (playerCount - b <= b) return playerCount - b;
    return 2 * playerCount - 3 * b;
}

std::string Run(const std::vector<std::string>&) {
    //return ToString(FindLeftWinner(3012210));
    return ToString(FindAcrossWinner(3012210));
}

bool RunTests() {
    if (FindLeftWinner(5) != 3) return false;
    if (FindAcrossWinner(5) != 2) return false;

    return true;
}