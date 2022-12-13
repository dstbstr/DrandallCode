#pragma once

#include "Common.h"

struct Disc {
    u32 Positions = 0;
    u32 Initial = 0;
};

constexpr bool Win(const std::vector<Disc>& discs, u32 ticks) {
    for (u32 i = 0; i < discs.size(); i++) {
        auto disc = discs[i];
        if ((disc.Initial + i + 1 + ticks) % disc.Positions != 0) {
            return false;
        }
    }

    return true;
}

static_assert(Win({ {5, 4}, {2, 1} }, 0) == false);
static_assert(Win({ {5, 4}, {2, 1} }, 5));

constexpr u32 BestSpin(const std::vector<Disc>& discs) {
    u32 spin = 0;
    while (!Win(discs, spin)) {
        spin++;
    }

    return spin;
}

std::string Run(const std::vector<std::string>&) {
    std::vector<Disc> discs = {
        {7, 0},
        {13, 0},
        {3, 2},
        {5, 2},
        {17, 0},
        {19, 7},
        {11, 0}
    };

    return ToString(BestSpin(discs));
}

bool RunTests() {
    std::vector<Disc> discs = { {5, 4}, {2, 1} };

    if (BestSpin(discs) != 5) return false;
    return true;
}