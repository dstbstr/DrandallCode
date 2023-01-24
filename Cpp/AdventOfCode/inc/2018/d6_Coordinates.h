#pragma once

#include "Common.h"

constexpr Coord ParseLine(const std::string& line) {
    auto split = Constexpr::Split(line, ", ");
    Coord result;
    Constexpr::ParseNumber(split[0], result.X);
    Constexpr::ParseNumber(split[1], result.Y);

    return result;
}

constexpr std::vector<u32> FindAreas(const std::vector<Coord>& coords, s32 area) {
    std::vector<u32> result;
    std::fill_n(std::back_inserter(result), coords.size(), 0u);

    for (s32 x = -area; x < area; x++) {
        for (s32 y = -area; y < area; y++) {
            s32 m = 100000;
            Coord point{ x, y };

            bool duplicate = false;
            size_t bestIndex = 0;
            for (size_t i = 0; i < coords.size(); i++) {
                auto dist = MDistance(point, coords[i]);
                if (dist == m) {
                    duplicate = true;
                }
                else if (dist < m) {
                    duplicate = false;
                    m = dist;
                    bestIndex = i;
                }
            }

            if (!duplicate) {
                result[bestIndex]++;
            }
        }
    }

    return result;
}

auto PartOne(const std::vector<std::string>& lines) {
    std::vector<Coord> coords;
    for (const auto& line : lines) {
        coords.push_back(ParseLine(line));
    }

    auto areas = FindAreas(coords, 400);
    auto areas2 = FindAreas(coords, 500);


    u32 maxArea = 0;
    for (size_t i = 0; i < coords.size(); i++) {
        auto coord = coords[i];
        if(areas[i] == areas2[i]) {
            maxArea = std::max(maxArea, areas[i]);
        }
    }

    return maxArea;
}

constexpr bool IsInRegion(const Coord& pos, std::vector<Coord> targets, s32 maxDistance) {
    s32 totalDistance = 0;
    for (const auto& target : targets) {
        totalDistance += MDistance(pos, target);
        if (totalDistance >= maxDistance) return false;
    }

    return true;
}

auto PartTwo(const std::vector<std::string>& lines, s32 maxDistance) {
    std::vector<Coord> coords;
    for (const auto& line : lines) {
        coords.push_back(ParseLine(line));
    }

    u32 count = 0;
    for (s32 x = -1000; x < 1000; x++) {
        for (s32 y = -1000; y < 1000; y++) {
            if (IsInRegion({ x, y }, coords, maxDistance)) {
                count++;
            }
        }
    }

    return count;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines, 10'000));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "1, 1",
        "1, 6",
        "8, 3",
        "3, 4",
        "5, 5",
        "8, 9"
    };

    if (PartOne(lines) != 17) return false;
    if (PartTwo(lines, 32) != 16) return false;
    return true;
}