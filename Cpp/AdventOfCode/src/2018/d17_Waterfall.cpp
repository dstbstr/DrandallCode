#include "2018/d17_Waterfall.h"
#include <bitset>

SOLUTION(2018, 17) {
    static constexpr size_t MaxX = 700;
    static constexpr size_t MaxY = 1700;
    using Map = std::array<std::array<char, MaxX>, MaxY>;

    struct Wall {
        size_t StartX{ 0 };
        size_t EndX{ 0 };
        size_t StartY{ 0 };
        size_t EndY{ 0 };
    };

    constexpr Wall ParseWall(std::string_view line) {
        auto split = Constexpr::Split(line, ", ");
        auto lhs = Constexpr::Split(split[0], "=");
        auto rhs = Constexpr::Split(split[1], "=");
        auto range = Constexpr::Split(rhs[1], "..");

        Wall wall;
        if (lhs[0][0] == 'x') {
            Constexpr::ParseNumber(lhs[1], wall.StartX);
            wall.EndX = wall.StartX;
            Constexpr::ParseNumber(range[0], wall.StartY);
            Constexpr::ParseNumber(range[1], wall.EndY);
        }
        else {
            Constexpr::ParseNumber(lhs[1], wall.StartY);
            wall.EndY = wall.StartY;
            Constexpr::ParseNumber(range[0], wall.StartX);
            Constexpr::ParseNumber(range[1], wall.EndX);
        }

        return wall;
    }

    constexpr void InitMap(const auto& lines, Map & outMap) {
        for (auto& row : outMap) {
            std::fill(row.begin(), row.end(), '.');
        }
        auto walls = ParseLines(lines, ParseWall);
        for (const auto& wall : walls) {
            for (auto y = wall.StartY; y <= wall.EndY; y++) {
                for (auto x = wall.StartX; x <= wall.EndX; x++) {
                    outMap[y][x] = '#';
                }
            }
        }
    }

    constexpr std::pair<RowCol, RowCol> FindLimits(const Map& map) {
        size_t minRow{ 999 }, maxRow{ 0 }, minCol{ 999 }, maxCol{ 0 };
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = 0; col < map[row].size(); col++) {
                if (map[row][col] != '.') {
                    minRow = std::min(minRow, row);
                    maxRow = std::max(maxRow, row);
                    minCol = std::min(minCol, col);
                    maxCol = std::max(maxCol, col);
                }
            }
        }

        return std::make_pair<RowCol, RowCol>({minRow, minCol}, {maxRow, maxCol});
    }

    static std::string PreviousLogLine;
    constexpr void PrintState(const Map& map) {
        auto [min, max] = FindLimits(map);

        std::vector<std::string> logLines;
        for (size_t row = min.Row; row <= max.Row; row++) {
            std::string logLine;
            for (size_t col = min.Col; col <= max.Col; col++) {
                logLine.push_back(map[row][col]);
            }
            logLines.push_back(logLine);
        }

        auto lines = Constexpr::JoinVec("\n", logLines);
        PreviousLogLine = lines;
        GET_LOGS().push_back(lines);
    }

    constexpr auto Solve(const auto& lines) {
        auto* mapPtr = new Map();
        auto& map = *mapPtr;

        InitMap(lines, map);
        auto [min, max] = FindLimits(map);

        auto start = RowCol{ 0, 500 };
        std::vector<RowCol> current {start};
        map[start.Row][start.Col] = '.';
        auto down = RowCol{ 1, 0 };

        while (!current.empty()) {
            auto pos = current.back();
            current.pop_back();

            auto& dChar = map[pos.Row + 1][pos.Col];
            auto dPos = pos + down;
            if(dChar == '.') {
                if (dPos.Row <= max.Row) {
                    if (dPos.Row >= min.Row) {
                        dChar = '|';
                    }
                    current.push_back(dPos);
                }
                continue;
            }
            else if (dChar == '|') continue;

            auto Fill = [&](s8 delta, size_t limit) {
                for (auto col = pos.Col; col != limit; col += delta) {
                    auto& currChar = map[pos.Row][col];
                    auto d = RowCol{ pos.Row + 1, col };
                    auto& nextChar = map[d.Row][d.Col];
                    if (currChar == '#') return true;

                    currChar = '|';
                    if (nextChar == '.') {
                        nextChar = '|';
                        current.push_back(d);
                        return false;
                    }
                    else if (nextChar == '|') {
                        return false;
                    }
                }
                return false;
            };

            auto supported = Fill(-1, 0);
            supported = Fill(1, 0) && supported;

            if (supported) {
                for (auto col = pos.Col; col > 0; col--) {
                    auto n = RowCol{ pos.Row, col };
                    if (map[n.Row][n.Col] != '#') {
                        map[n.Row][n.Col] = '~';
                    }
                    else {
                        break;
                    }
                }
                for (auto col = pos.Col + 1; col < MaxX; col++) {
                    auto n = RowCol{ pos.Row, col };
                    if (map[n.Row][n.Col] != '#') {
                        map[n.Row][n.Col] = '~';
                    }
                    else {
                        break;
                    }
                }

                current.push_back({ pos.Row - 1, pos.Col });
            }
            
        }

        std::pair<size_t, size_t> result;
        for (const auto& row : map) {
            result.first += std::count(row.begin(), row.end(), '~');
            result.second += std::count(row.begin(), row.end(), '|');
        }
        delete mapPtr;

        return result;
    }


    PART_ONE() {
        auto [lhs, rhs] = Solve(lines);

        return Constexpr::ToString(lhs + rhs);
    }

    PART_TWO() {
        auto [lhs, rhs] = Solve(lines);

        return Constexpr::ToString(lhs);
    }

    TESTS() {
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

        auto [lhs, rhs] = Solve(lines);
        if (lhs != 29) return false;
        if (rhs != 28) return false;
        
        lines = {
            "x=499, y=3..4",
            "y=4, x=499..501",
            "x=501, y=3..4",
            "x=494, y=6..8",
            "y=8, x=494..506",
            "x=506, y=6..8"
        };

        auto [lhs2, rhs2] = Solve(lines);
        if (lhs2 + rhs2 != 53) return false;

        return true;
    }
}