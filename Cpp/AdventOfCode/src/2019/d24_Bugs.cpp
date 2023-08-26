#include "2019/d24_Bugs.h"
#include <set>

SOLUTION(2019, 24) {
    using Grid = std::array<std::array<bool, 5>, 5>;

    constexpr Grid ParseGrid(const auto& lines) {
        Grid result{};
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                result[row][col] = lines[row][col] == '#';
            }
        }

        return result;
    }

    constexpr RowCol Max = { 4, 4 };

    constexpr size_t CountNeighborBugs(const RowCol & pos, const Grid & current) {
        auto countBugs = [&](const RowCol& pos) {
            return current[pos.Row][pos.Col];
        };

        auto neighbors = GetDirectNeighbors(pos, Max);
        return std::count_if(neighbors.cbegin(), neighbors.cend(), countBugs);
    }

    constexpr bool NextForPos(bool isBug, size_t neighborBugs) {
        if (isBug) {
            return neighborBugs == 1;
        }
        else {
            return neighborBugs == 1 || neighborBugs == 2;
        }
    }

    constexpr Grid Next(const Grid & current) {
        Grid next;
        for (size_t row = 0; row < 5; row++) {
            for (size_t col = 0; col < 5; col++) {
                auto neighborBugs = CountNeighborBugs({ row, col }, current);
                next[row][col] = NextForPos(current[row][col], neighborBugs);
            }
        }

        return next;
    }

    constexpr u64 CalcDiversity(const Grid & grid) {
        u64 result = 0;
        u64 exp = 0;
        u64 base = 2;
        for (size_t row = 0; row < 5; row++) {
            for (size_t col = 0; col < 5; col++) {
                if (grid[row][col]) {
                    result += Constexpr::Pow(base, exp);
                }
                exp++;
            }
        }

        return result;
    }


    static constexpr std::array<RowCol, 4> InnerPositions = { { {1, 1}, {1, 3}, {3, 1}, {3, 3} } };
    constexpr size_t CountNeighborBugs(const RowCol & pos, size_t currentLevel, const std::vector<Grid>&levels) {
        size_t neighborBugs = CountNeighborBugs(pos, levels[currentLevel]);

        if (pos.Row == 0 || pos.Row == 4 || pos.Col == 0 || pos.Col == 4) {
            if (currentLevel > 0) {
                auto otherLevel = levels[currentLevel - 1];
                if (pos.Row == 0) {
                    neighborBugs += otherLevel[1][2];
                }
                else if (pos.Row == 4) {
                    neighborBugs += otherLevel[3][2];
                }

                if (pos.Col == 0) {
                    neighborBugs += otherLevel[2][1];
                }
                else if (pos.Col == 4) {
                    neighborBugs += otherLevel[2][3];
                }
            }
        }
        else if (std::find(InnerPositions.begin(), InnerPositions.end(), pos) == InnerPositions.end()) {
            if (currentLevel < levels.size() - 1) {
                auto otherLevel = levels[currentLevel + 1];
                if (pos.Row == 1) {
                    neighborBugs += otherLevel[0][0];
                    neighborBugs += otherLevel[0][1];
                    neighborBugs += otherLevel[0][2];
                    neighborBugs += otherLevel[0][3];
                    neighborBugs += otherLevel[0][4];
                }
                else if (pos.Row == 3) {
                    neighborBugs += otherLevel[4][0];
                    neighborBugs += otherLevel[4][1];
                    neighborBugs += otherLevel[4][2];
                    neighborBugs += otherLevel[4][3];
                    neighborBugs += otherLevel[4][4];
                }
                else if (pos.Col == 1) {
                    neighborBugs += otherLevel[0][0];
                    neighborBugs += otherLevel[1][0];
                    neighborBugs += otherLevel[2][0];
                    neighborBugs += otherLevel[3][0];
                    neighborBugs += otherLevel[4][0];
                }
                else if (pos.Col == 3) {
                    neighborBugs += otherLevel[0][4];
                    neighborBugs += otherLevel[1][4];
                    neighborBugs += otherLevel[2][4];
                    neighborBugs += otherLevel[3][4];
                    neighborBugs += otherLevel[4][4];
                }
            }
        }

        return neighborBugs;
    }

    constexpr std::vector<Grid> Next(const std::vector<Grid>&current) {
        const RowCol Center = { 2, 2 };

        std::vector<Grid> nextLevels = current;
        for (auto level = 0; level < current.size(); level++) {
            for (size_t row = 0; row < 5; row++) {
                for (size_t col = 0; col < 5; col++) {
                    RowCol pos = { row, col };
                    if (pos == Center) continue;
                    auto neighborBugs = CountNeighborBugs(pos, level, current);
                    nextLevels[level][row][col] = NextForPos(current[level][row][col], neighborBugs);
                }
            }
        }

        return nextLevels;
    }

    constexpr size_t CountBugs(const std::vector<Grid>&levels) {
        size_t result = 0;
        for (const auto& level : levels) {
            for (auto row : level) {
                for (auto isBug : row) {
                    result += isBug;
                }
            }
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto grid = ParseGrid(lines);
        Constexpr::SmallSet<Grid> seen;
        seen.insert(grid);

        while (true) {
            grid = Next(grid);
            if (seen.contains(grid)) {
                break;
            }
            else {
                seen.insert(grid);
            }
        }

        return CalcDiversity(grid);
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    constexpr auto SolvePartTwo(const auto& lines, size_t minutes) {
        std::vector<Grid> levels;
        for (auto i = 0; i < minutes + 2; i++) {
            levels.push_back(Grid{});
        }

        levels[(minutes / 2) + 1] = ParseGrid(lines);

        for (auto minute = 0; minute < minutes; minute++) {
            levels = Next(levels);
        }

        return CountBugs(levels);
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(Lines, 200));
    }

    TESTS() {
        static_assert(CalcDiversity(ParseGrid(std::array<std::string_view, 5>{
            ".....",
                ".....",
                ".....",
                "#....",
                ".#..."
        })) == 2129920);
        
        std::vector<std::string> lines = {
            "....#",
            "#..#.",
            "#..##",
            "..#..",
            "#...."
        };

        if (SolvePartOne(lines) != 2129920) return false;
        if (SolvePartTwo(lines, 10) != 99) return false;

        return true;
    }
}