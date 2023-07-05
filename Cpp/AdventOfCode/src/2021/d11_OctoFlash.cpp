#include "2021/d11_OctoFlash.h"

SOLUTION(2021, 11) {
    struct Octo {
        size_t Energy;
        bool HasFlashed;
    };

    using Grid = std::vector<std::vector<Octo>>;

    constexpr Grid ParseInput(const std::vector<std::string>&lines) {
        Grid result;
        for (const auto& line : lines) {
            std::vector<Octo> row;
            for (auto c : line) {
                size_t energy = static_cast<size_t>(c - '0');
                row.push_back(Octo{ energy, false });
            }
            result.push_back(row);
        }
        return result;
    }

    constexpr void Next(Grid & grid) {
        RowCol limits = { grid.size() - 1, grid[0].size() - 1 };

        Constexpr::ApplyToMatrixValue(grid, [](Octo& o) { o.Energy++; });

        bool flashed = true;
        while (flashed) {
            flashed = false;
            Constexpr::ApplyToMatrixIndex(grid, [&](size_t row, size_t col) {
                auto& o = grid[row][col];
                if (o.HasFlashed || o.Energy < 10) return;
                auto neighbors = GetAllNeighbors({ row, col }, limits);
                for (auto neighbor : neighbors) {
                    grid[neighbor.Row][neighbor.Col].Energy++;
                }
                o.HasFlashed = true;
                flashed = true;
                });
        }
    }

    constexpr size_t CountAndReset(Grid & grid) {
        size_t result = 0;
        for (auto& row : grid) {
            for (auto& val : row) {
                if (val.HasFlashed) {
                    result++;
                    val.Energy = 0;
                    val.HasFlashed = false;
                }
            }
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines, size_t steps) {
        auto grid = ParseInput(lines);
        size_t result = 0;
        for (auto i = 0; i < steps; i++) {
            Next(grid);
            result += CountAndReset(grid);
        }

        return result;
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto grid = ParseInput(lines);
        size_t step = 0;
        while (true) {
            step++;
            Next(grid);
            if (CountAndReset(grid) == 100) break;
        }

        return step;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 100));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "5483143223",
            "2745854711",
            "5264556173",
            "6141336146",
            "6357385478",
            "4167524645",
            "2176841721",
            "6882881134",
            "4846848554",
            "5283751526"
        };

        if (Part1(lines, 1) != 0) return false;
        if (Part1(lines, 2) != 35) return false;
        if (Part1(lines, 10) != 204) return false;
        if (Part1(lines, 100) != 1656) return false;

        if (Part2(lines) != 195) return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}