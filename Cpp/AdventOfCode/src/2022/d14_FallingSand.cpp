#include "2022/d14_FallingSand.h"

SOLUTION(2022, 14) {

    //X = 465 - 528
    //Y = 12 - 178
    constexpr size_t Rows = 200;
    constexpr size_t Cols = 1000;
    constexpr UCoord Entrance{ 500, 0 };

    using Map = std::array<std::array<bool, Cols>, Rows>;

    constexpr void Set(size_t x, size_t y, Map & map) {
        map[y][x] = true;
    }

    constexpr bool Check(size_t x, size_t y, const Map & map) {
        return map[y][x];
    }

    constexpr void AddWall(UCoord start, UCoord end, Map & map) {
        if (start.X == end.X) {
            size_t a, b;
            a = start.Y < end.Y ? start.Y : end.Y;
            b = start.Y < end.Y ? end.Y : start.Y;

            for (; a <= b; a++) {
                Set(start.X, a, map);
            }
        }
        else {
            size_t a, b;
            a = start.X < end.X ? start.X : end.X;
            b = start.X < end.X ? end.X : start.X;

            for (; a <= b; a++) {
                Set(a, start.Y, map);
            }
        }
    }

    constexpr void AddWalls(const auto& lines, Map & map) {
        for (const auto& line : lines) {
            auto points = Constexpr::Split(line, " -> ");
            std::vector<UCoord> coords;
            for (auto point : points) {
                u32 x, y;
                auto split = Constexpr::Split(point, ",");
                Constexpr::ParseNumber(split[0], x);
                Constexpr::ParseNumber(split[1], y);
                coords.push_back({ x, y });
            }

            for (size_t index = 0; index < coords.size() - 1; index++) {
                AddWall(coords[index], coords[index + 1], map);
            }
        }
    }

    constexpr void AddFloor(Map & map, u32 maxY) {
        UCoord start = { 0, maxY };
        UCoord end = { 999, maxY };

        AddWall(start, end, map);
    }

    constexpr bool DropSand(UCoord & sandPos, Map & map) {
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

    constexpr u32 CountSand(const auto& lines, u32 maxY, auto IsDone) {
        auto* map = new Map();
        AddWalls(lines, *map);
        AddFloor(*map, maxY);
        u32 grains = 0;
        UCoord sandPos = Entrance;
        while (true) {
            sandPos = Entrance;
            grains++;
            while (!DropSand(sandPos, *map) && !IsDone(sandPos));
            if (IsDone(sandPos)) {
                delete map;
                return grains;
            }
        }
        delete map;
        return grains;
    }

    PART_ONE() {
        return Constexpr::ToString(CountSand(Lines, 180, [](UCoord sandPos) {
            return sandPos.Y == 178;
            }) - 1);
    }

    PART_TWO() {
        return Constexpr::ToString(CountSand(Lines, 180, [](UCoord sandPos) {
            return sandPos == Entrance;
            }));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "498,4 -> 498,6 -> 496,6",
            "503,4 -> 502,4 -> 502,9 -> 494,9"
        };

        if (CountSand(lines, 11, [](UCoord sandPos) { return sandPos.Y == 9; }) - 1 != 24) return false;
        if (CountSand(lines, 11, [](UCoord sandPos) { return sandPos == Entrance; }) != 93) return false;
        return true;
    }
}