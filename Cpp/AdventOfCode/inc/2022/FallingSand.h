#pragma once

#include "Common.h"

//X = 465 - 528
//Y = 12 - 178
constexpr size_t Rows = 200;
constexpr size_t Cols = 1000;
using Map = std::array<std::array<bool, Cols>, Rows>;

constexpr void Set(u32 x, u32 y, Map& map) {
    map[y][x] = true;
}

constexpr bool Check(u32 x, u32 y, const Map& map) {
    return map[y][x];
}

constexpr void AddWall(UCoord start, UCoord end, Map& map) {
    if (start.X == end.X) {
        u32 a, b;
        a = start.Y < end.Y ? start.Y : end.Y;
        b = start.Y < end.Y ? end.Y : start.Y;

        for (; a <= b; a++) {
            Set(start.X, a, map);
        }
    }
    else {
        u32 a, b;
        a = start.X < end.X ? start.X : end.X;
        b = start.X < end.X ? end.X : start.X;

        for (; a <= b; a++) {
            Set(a, start.Y, map);
        }
    }
}

constexpr void AddWalls(const std::vector<std::string>& lines, Map& map) {
    for (const auto& line : lines) {
        auto points = StrUtil::Split(line, " -> ");
        std::vector<UCoord> coords;
        for (auto point : points) {
            u32 x, y;
            auto split = StrUtil::Split(point, ",");
            ParseNumber(split[0], x);
            ParseNumber(split[1], y);
            coords.push_back({ x, y });
        }

        for (size_t index = 0; index < coords.size() - 1; index++) {
            AddWall(coords[index], coords[index + 1], map);
        }
    }
}

constexpr void AddFloor(Map& map, u32 maxY) {
    UCoord start = { 0, maxY };
    UCoord end = { 999, maxY };

    AddWall(start, end, map);
}

bool DropSand(UCoord& sandPos, Map& map) {
    if (Check(sandPos.X, sandPos.Y + 1, map)) { // down
        if (Check(sandPos.X - 1, sandPos.Y + 1, map)) { //down left
            if (Check(sandPos.X + 1, sandPos.Y + 1, map)) { //down right
                Set(sandPos.X, sandPos.Y, map);
                return true;
            }
            else {
                sandPos.X++;
            }
        }
        else {
            sandPos.X--;
        }
    }

    sandPos.Y++;
    return false;
}

constexpr bool IsAbyss(UCoord sandPos) {
    return sandPos.Y > 180;
}

constexpr bool BlocksEntrance(UCoord sandPos) {
    return sandPos.Y == 0 && sandPos.X == 500;
}

u32 CountSand(const std::vector<std::string>& lines, u32 maxY) {
    auto map = std::make_unique<Map>();
    AddWalls(lines, *map);
    AddFloor(*map, maxY);
    u32 grains = 0;

    UCoord sandPos;
    while (true) {
        sandPos = { 500, 0 };
        /*
        while (!DropSand(sandPos, map) && !IsAbyss(sandPos));
        if (IsAbyss(sandPos)) {
            return grains;
        }
        */

        grains++;

        while (!DropSand(sandPos, *map) && !BlocksEntrance(sandPos));
        if (BlocksEntrance(sandPos)) {
            return grains;
        }
    }

    return grains;
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(CountSand(lines, 180));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "498,4 -> 498,6 -> 496,6",
        "503,4 -> 502,4 -> 502,9 -> 494,9"
    };

    if (CountSand(lines, 11) != 93) return false;
    return true;
}