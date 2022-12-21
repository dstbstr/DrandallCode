#pragma once

#include "Common.h"

using Grid = std::array<std::array<std::array<bool, 25>, 25>, 25>;

struct Vec3 {
    size_t X;
    size_t Y;
    size_t Z;
};

void ParseLine(const std::string& line, Grid& grid) {
    size_t x, y, z;
    auto split = Constexpr::Split(line, ",");
    ParseNumber(split[0], x);
    ParseNumber(split[1], y);
    ParseNumber(split[2], z);

    grid[x][y][z] = true;
}

constexpr u32 CalcSurface(const Grid& grid) {
    u32 result = 0;
    for (size_t x = 0; x < 25; x++) {
        for (size_t y = 0; y < 25; y++) {
            for (size_t z = 0; z < 25; z++) {
                if (!grid[x][y][z]) continue;
                if (x == 0 || x == 24) result++;
                else {
                    if (!grid[x - 1][y][z]) result++;
                    if (!grid[x + 1][y][z]) result++;
                }
                if (y == 0 || y == 24) result++;
                else {
                    if (!grid[x][y - 1][z]) result++;
                    if (!grid[x][y + 1][z]) result++;
                }
                if (z == 0 || z == 24) result++;
                else {
                    if (!grid[x][y][z - 1]) result++;
                    if (!grid[x][y][z + 1]) result++;
                }

            }
        }
    }

    return result;
}

constexpr void FloodFill(const Grid& grid, Grid& steamGrid) {
    std::vector<Vec3> current;
    std::vector<Vec3> next;
    current.push_back({0, 0, 0});

    while (!current.empty()) {
        for (const auto& pos : current) {
            if (grid[pos.X][pos.Y][pos.Z]) continue;
            if (steamGrid[pos.X][pos.Y][pos.Z]) continue;
            steamGrid[pos.X][pos.Y][pos.Z] = true;

            if (pos.X > 0) {
                next.push_back({ pos.X - 1, pos.Y, pos.Z });
            }
            if (pos.X < 24) {
                next.push_back({ pos.X + 1, pos.Y, pos.Z });
            }
            if (pos.Y > 0) {
                next.push_back({ pos.X, pos.Y - 1, pos.Z });
            }
            if (pos.Y < 24) {
                next.push_back({ pos.X, pos.Y + 1, pos.Z });
            }
            if (pos.Z > 0) {
                next.push_back({ pos.X, pos.Y, pos.Z - 1 });
            }
            if (pos.Z < 24) {
                next.push_back({ pos.X, pos.Y, pos.Z + 1 });
            }
        }

        current = next;
        next.clear();
    }
}

constexpr u32 CalcAirPockets(const Grid& grid) {
    Grid steamGrid{};
    FloodFill(grid, steamGrid);

    Grid spaceGrid{};

    for (size_t x = 0; x < 25; x++) {
        for (size_t y = 0; y < 25; y++) {
            for (size_t z = 0; z < 25; z++) {
                spaceGrid[x][y][z] = !(grid[x][y][z] || steamGrid[x][y][z]);
            }
        }
    }

    return CalcSurface(spaceGrid);
}
constexpr u32 CalcExterior(const Grid& grid) {
    return CalcSurface(grid) - CalcAirPockets(grid);
}

std::string Run(const std::vector<std::string>& lines) {
    Grid grid{};
    for (const auto& line : lines) {
        ParseLine(line, grid);
    }

    //between 2570 and 4316
    
    //return ToString(CalcSurface(grid));
    return ToString(CalcExterior(grid));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "2,2,2",
        "1,2,2",
        "3,2,2",
        "2,1,2",
        "2,3,2",
        "2,2,1",
        "2,2,3",
        "2,2,4",
        "2,2,6",
        "1,2,5",
        "3,2,5",
        "2,1,5",
        "2,3,5"
    };
    Grid grid{};
    for (const auto& line : lines) {
        ParseLine(line, grid);
    }

    if (CalcSurface(grid) != 64) return false;
    if (CalcExterior(grid) != 58) return false;
    return true;
}