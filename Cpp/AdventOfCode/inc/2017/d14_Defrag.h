#pragma once

#include "Common.h"
#include "KnotHash.h"

#include <bitset>

auto PartOne(const std::string& key) {
    u32 used = 0;
    for (auto i = 0; i < 128; i++) {
        std::string toHash = key + "-" + ToString(i);
        auto hash = KnotHash::Hash(toHash);
        for (auto value : hash) {
            used += std::popcount(value);
        }
    }

    return used;
}

using Grid = std::vector<std::vector<bool>>;

constexpr Grid CreateGrid(const std::string& key) {
    Grid result;
    for (auto i = 0; i < 128; i++) {
        std::string toHash = key + "-" + ToString(i);
        auto hash = KnotHash::Hash(toHash);
        std::vector<bool> line;

        for (auto value : hash) {
            std::bitset<8> bits{ value };
            for (auto index = 0; index < 8; index++) {
                line.push_back(bits[7 - index]);
            }
        }

        result.push_back(line);
    }

    return result;
}

constexpr bool FindGroupStart(const Grid& grid, RowCol& out) {
    for (size_t row = 0; row < grid.size(); row++) {
        for (size_t col = 0; col < grid[row].size(); col++) {
            if (grid[row][col]) {
                out = { row, col };
                return true;
            }
        }
    }

    return false;
}

constexpr void FloodFill(Grid& grid, const RowCol& start) {
    auto limits = RowCol{ 127, 127 };
    std::vector<RowCol> current;
    current.push_back(start);

    while (!current.empty()) {
        auto pos = current.back();
        current.pop_back();
        grid[pos.Row][pos.Col] = false;
        auto neighbors = Get4Neighbors(pos, limits);
        std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(current), [&grid](const RowCol& rc) {
            return grid[rc.Row][rc.Col];
            });
    }
}

auto PartTwo(const std::string& line) {
    u32 groups = 0;
    auto grid = CreateGrid(line);
    auto pos = RowCol{ 0, 0 };
    while (FindGroupStart(grid, pos)) {
        groups++;
        FloodFill(grid, pos);
    }

    return groups;
}

std::string Run(const std::vector<std::string>&) {
    //return ToString(PartOne("hfdlxzhv"));
    return ToString(PartTwo("hfdlxzhv"));
}

bool RunTests() {
    //if (PartOne("flqrgnkx") != 8108) return false;
    if (PartTwo("flqrgnkx") != 1242) return false;
    return true;
}