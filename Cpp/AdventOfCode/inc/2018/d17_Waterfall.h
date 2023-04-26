#pragma once

#include "Common.h"
#include <bitset>
#include <iostream>

static constexpr size_t MaxX = 700;
static constexpr size_t MaxY = 2000;

using Map = std::bitset<MaxY * MaxX>;

void Fill(UCoord coord, Map& map) {
    map[coord.Y * MaxX + coord.X] = true;
}

constexpr bool IsFilled(UCoord coord, const Map& map) {
    return map[coord.Y * MaxX + coord.X];
}

constexpr UCoord GetMin(const Map& walls) {
    UCoord min{ MaxX, MaxY};
    for (u32 x = 0; x < MaxX; x++) {
        for (u32 y = 0; y < MaxY; y++) {
            UCoord pos = { x, y };
            if (IsFilled(pos, walls)) {
                min.X = std::min(min.X, pos.X);
                min.Y = std::min(min.Y, pos.Y);
            }
        }
    }

    return min;
}

constexpr UCoord GetMax(const Map& walls) {
    UCoord max{ 0, 0 };
    for (u32 x = 0; x < MaxX; x++) {
        for (u32 y = 0; y < MaxY; y++) {
            UCoord pos = { x, y };
            if (IsFilled(pos, walls)) {
                max.X = std::max(max.X, pos.X);
                max.Y = std::max(max.Y, pos.Y);
            }
        }
    }

    return max;
}

void ParseLine(const std::string& line, Map& map) {
    auto split = Constexpr::Split(line, ", ");
    auto lhs = Constexpr::Split(split[0], "=");
    auto rhs = Constexpr::Split(split[1], "=");
    auto range = Constexpr::Split(rhs[1], "..");

    UCoord coord;
    if (lhs[0][0] == 'x') {
        Constexpr::ParseNumber(lhs[1], coord.X);
    }
    else {
        Constexpr::ParseNumber(lhs[1], coord.Y);
    }

    u32 start, end;
    Constexpr::ParseNumber(range[0], start);
    Constexpr::ParseNumber(range[1], end);
    for (; start <= end; start++) {
        if (rhs[0][0] == 'x') {
            coord.X = start;
        }
        else {
            coord.Y = start;
        }

        Fill(coord, map);
    }
}

Map ParseLines(const std::vector<std::string>& lines) {
    Map result;
    for (const auto& line : lines) {
        ParseLine(line, result);
    }

    return result;
}

constexpr bool CanMove(const UCoord& pos, const Map& walls, const Map& water) {
    if (pos.Y >= MaxY || pos.Y < 0 || pos.X == 0 || pos.X >= MaxX) return false;
    return !IsFilled(pos, walls) && !IsFilled(pos, water);
}

constexpr bool CanStop(const UCoord& pos, const Map& walls, const Map& water) {
    if (pos.X == 0 || pos.X == MaxX || pos.Y == 0 || pos.Y == MaxY) return false;

    bool right = false, left = false;
    for (auto x = pos.X; x < MaxX; x++) {
        if (IsFilled({ x, pos.Y }, walls)) {
            right = true;
            break;
        }
        else if (!IsFilled({ x, pos.Y + 1 }, walls) && !IsFilled({ x, pos.Y + 1 }, water)) {
            return false;
        }
    }
    if (!right) return false;
    for (auto x = pos.X; x > 0; x--) {
        if (IsFilled({ x, pos.Y }, walls)) {
            left = true;
            break;
        }
        else if (!IsFilled({ x, pos.Y + 1 }, walls) && !IsFilled({ x, pos.Y + 1 }, water)) {
            return false;
        }
    }
    return left;
}

constexpr UCoord GoDown(const UCoord& pos) {
    return { pos.X, pos.Y + 1 };
}
constexpr UCoord GoLeft(const UCoord& pos) {
    return { pos.X - 1, pos.Y };
}
constexpr UCoord GoRight(const UCoord& pos) {
    return { pos.X + 1, pos.Y };
}

bool FloodFill(const UCoord& start, const Map& walls, Map& water) {
    static auto max = GetMax(walls);

    std::vector<UCoord> current;
    std::unordered_set<UCoord> seen;
    current.push_back(start);
    seen.insert(start);
    bool filled = false;
    while (!current.empty()) {
        auto pos = current.back();
        current.pop_back();
        seen.insert(pos);
        if (CanMove(GoDown(pos), walls, water)) {
            if (pos.Y < max.Y) {
                current.push_back(GoDown(pos));
            }
        }
        else {
            if (CanStop(pos, walls, water)) {
                Fill(pos, water);
                filled = true;
            }

            auto left = GoLeft(pos);
            auto right = GoRight(pos);
            if (seen.find(left) == seen.end() && CanMove(left, walls, water)) {
                current.push_back(left);
            }
            if (seen.find(right) == seen.end() && CanMove(right, walls, water)) {
                current.push_back(right);
            }
        }
    }

    return filled;
}

size_t CountRunoff(UCoord& start, const Map& walls, const Map& water) {
    static auto max = GetMax(walls);
    static auto min = GetMin(walls);

    std::vector<UCoord> current;
    std::unordered_set<UCoord> seen;
    current.push_back(start);

    while (!current.empty()) {
        auto pos = current.back();
        current.pop_back();
        if (pos.Y >= min.Y) {
            seen.insert(pos);
        }
        if (CanMove(GoDown(pos), walls, water)) {
            if (pos.Y < max.Y) {
                current.push_back(GoDown(pos));
            }
        }
        else {
            auto left = GoLeft(pos);
            auto right = GoRight(pos);
            if (seen.find(left) == seen.end() && CanMove(left, walls, water)) {
                current.push_back(left);
            }
            if (seen.find(right) == seen.end() && CanMove(right, walls, water)) {
                current.push_back(right);
            }
        }
    }

    return seen.size();
}

void PrintState(const Map& walls, const Map& water) {
    auto min = GetMin(walls);
    auto max = GetMax(walls);

    for (auto y = min.Y; y <= max.Y; y++) {
        for (auto x = min.X; x <= max.X; x++) {
            UCoord pos = { x, y };
            if (IsFilled(pos, walls)) std::cout << '#';
            else if (IsFilled(pos, water)) std::cout << '~';
            else std::cout << '.';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

auto PartOne(const std::vector<std::string>& lines) {
    auto walls = ParseLines(lines);
    Map water;
    UCoord source = { 500, 0 };
    while (FloodFill(source, walls, water)) {
        //PrintState(walls, water);
    }

    return water.count() + CountRunoff(source, walls, water);
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto walls = ParseLines(lines);
    Map water;
    UCoord source = { 500, 0 };
    while (FloodFill(source, walls, water));

    return water.count();
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "x=495, y=2..7",
        "y=7, x=495..501",
        "x=501, y=3..7",
        "x=498, y=2..4",
        "x=506, y=1..2",
        "x=498, y=10..13",
        "x=504, y=10..13",
        "y=13, x=498..504"
    };

    if (PartOne(lines) != 57) return false;
    return true;
}