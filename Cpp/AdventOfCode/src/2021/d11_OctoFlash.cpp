#include "2021/d11_OctoFlash.h"

SOLUTION(2021, 11) {
    struct Octo {
        size_t Energy{ 0 };
        bool HasFlashed{ false };
    };

    using Grid = std::vector<std::vector<Octo>>;

    std::vector<Octo> ParseLine(const std::string& line) {
        std::vector<Octo> result;
        for (auto c : line) {
            result.push_back({ static_cast<size_t>(c - '0') });
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

    constexpr void Solve(const std::vector<std::string>& lines, auto IsDone) {
        auto grid = ParseLines(lines, ParseLine);
        while(true) {
            Next(grid);
            if (IsDone(CountAndReset(grid))) break;
        }
    }

    PART_ONE() {
        size_t step = 0;
        size_t result = 0;
        Solve(lines, [&](size_t r) {
            result += r;
            step++;
            return step == 100;
            });
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        size_t step = 0;
        Solve(lines, [&](size_t r) {
            step++;
            return r == 100;
            });
        return Constexpr::ToString(step);
    }

    TESTS() {
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

        if (PartOne(lines) != "1656") return false;
        if (PartTwo(lines) != "195") return false;
        return true;
    }
}