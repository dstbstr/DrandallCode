#pragma once

#include "Common.h"

std::unordered_map<u32, Coord> CreateSpiral(u32 maxValue) {
    std::unordered_map<u32, Coord> result;
    u32 run = 1;
    s32 delta = 1;
    Coord coord{ 0, 0 };
    result[1] = coord;

    u32 val = 2;
    auto shiftX = [&]() {
        for (u32 x = 0; x < run; x++) {
            coord.X += delta;
            result[val] = coord;
            val++;
        }
    };
    auto shiftY = [&]() {
        for (u32 y = 0; y < run; y++) {
            coord.Y += delta;
            result[val] = coord;
            val++;
        }
    };
    auto turnCorner = [&]() {
        run++;
        delta = -delta;
    };

    while (val <= maxValue) {
        shiftX();
        shiftY();

        turnCorner();

        shiftX();
        shiftY();

        turnCorner();
    }

    return result;
}

std::unordered_map<Coord, u32, CoordHash> WriteSummingSpiral(const std::unordered_map<u32, Coord>& spiral) {
    static const Coord min = { -1000, -1000 };
    static const Coord max = { 1000, 1000 };

    std::unordered_map<Coord, u32, CoordHash> result;
    result[spiral.at(1)] = 1;
    for (auto key = 2; key < spiral.size(); key++) {
        auto coord = spiral.at(key);
        auto neighbors = GetAllNeighbors(coord, max, min);
        u32 value = 0;
        for (auto neighbor : neighbors) {
            if (result.find(neighbor) != result.end()) {
                value += result.at(neighbor);
            }
        }
        result[coord] = value;
    }

    return result;
}

auto PartOne(u32 target) {
    auto spiral = CreateSpiral(target);
    return MDistance(spiral.at(target), { 0, 0 });
}

auto PartTwo(u32 target) {
    auto spiral = CreateSpiral(1000);
    auto sumSpiral = WriteSummingSpiral(spiral);

    for (auto key = 1; key < 1000; key++) {
        auto coord = spiral[key];
        if (sumSpiral[coord] > target) {
            return sumSpiral[coord];
        }
    }

    return 0u;
}

std::string Run(const std::vector<std::string>&) {
    //return ToString(PartOne(347991));
    return ToString(PartTwo(347991));
}

bool RunTests() {
    if (PartOne(1) != 0) return false;
    if (PartOne(12) != 3) return false;
    if (PartOne(23) != 2) return false;
    if (PartOne(1024) != 31) return false;

    if (PartTwo(20) != 23) return false;
    if (PartTwo(122) != 133) return false;
    return true;
}