#include "2020/d3_TreePath.h"

SOLUTION(2020, 3) {
    using Grid = std::vector<std::vector<bool>>;

    constexpr Grid ParseGrid(const std::vector<std::string>&lines) {
        Grid result;
        for (const auto& line : lines) {
            std::vector<bool> row;
            for (auto c : line) {
                row.push_back(c == '#');
            }
            result.push_back(row);
        }
        return result;
    }

    constexpr bool MoveOnSlope(const Grid & grid, RowCol & pos, RowCol slope) {
        pos.Row = (pos.Row + slope.Row) % grid.size();
        pos.Col = (pos.Col + slope.Col) % grid[pos.Row].size();

        return grid[pos.Row][pos.Col];
    }

    constexpr size_t CountTreesOnPath(const Grid & grid, RowCol slope) {
        RowCol pos = { 0, 0 };
        size_t result = 0;
        while (pos.Row < grid.size() - 1) {
            result += MoveOnSlope(grid, pos, slope);
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto grid = ParseGrid(lines);
        RowCol slope = { 1, 3 };
        return CountTreesOnPath(grid, slope);
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto grid = ParseGrid(lines);
        std::vector<RowCol> slopes = {
            {1, 1},
            {1, 3},
            {1, 5},
            {1, 7},
            {2, 1}
        };
        size_t result = 1;
        for (const auto& slope : slopes) {
            result *= CountTreesOnPath(grid, slope);
        }

        return result;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "..##.......",
            "#...#...#..",
            ".#....#..#.",
            "..#.#...#.#",
            ".#...##..#.",
            "..#.##.....",
            ".#.#.#....#",
            ".#........#",
            "#.##...#...",
            "#...##....#",
            ".#..#...#.#"
        };

        if (Part1(lines) != 7) return false;
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