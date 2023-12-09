#include "2021/d15_Chiton.h"
#include "Algorithms/AStar.h"

SOLUTION(2021, 15) {
    using Map = std::vector<std::vector<u32>>;
    constexpr Map ParseInput(const auto& lines) {
        Map numbers;
        for (const auto& line : lines) {
            std::vector<u32> current;
            for (auto c : line) {
                current.push_back(c - '0');
            }
            numbers.push_back(current);
        }

        return numbers;
    }

    constexpr Map ExpandMap(const Map & original) {
        Map result;
        for (size_t tileRow = 0; tileRow < 5; tileRow++) {
            for (size_t row = 0; row < original.size(); row++) {
                std::vector<u32> current;

                for (size_t tileCol = 0; tileCol < 5; tileCol++) {
                    for (size_t col = 0; col < original[row].size(); col++) {
                        u32 val = original[row][col] + static_cast<u32>(tileRow + tileCol);
                        if (val > 9) val -= 9;
                        current.push_back(val);
                    }
                }
                result.push_back(current);
            }
        }

        return result;
    }

    constexpr size_t FindAstar(const Map& map, RowCol limits) {
        RowCol origin = { 0, 0 };
        RowCol target = { map.size() - 1, map[0].size() - 1 };

        auto cost = [&](RowCol, RowCol to) { return map[to.Row][to.Col]; };
        auto done = [&](RowCol pos) { return pos == target; };
        auto h = [&](RowCol pos) { return MDistance(pos, target); };
        auto n = [&](RowCol pos) { return GetDirectNeighbors(pos, limits); };
        auto move = [](RowCol, RowCol) {};
        auto path = AStarMin<RowCol, true>(origin, cost, done, h, n, move);

        size_t result = 0;
        for (size_t i = 1; i < path.size(); i++) {
            auto pos = path[i];
            result += map[pos.Row][pos.Col];
        }
        return result;
    }

    constexpr size_t Solve(const Map& map) {
        RowCol limits = { map.size() - 1, map[0].size() - 1 };
        return FindAstar(map, limits);
    }

    PART_ONE() {
        auto map = ParseInput(lines);
        return Constexpr::ToString(Solve(map));
    }

    PART_TWO() {
        auto map = ParseInput(lines);
        auto bigMap = ExpandMap(map);
        return Constexpr::ToString(Solve(bigMap));
    }

    TESTS() {
        /*
        std::vector<std::string> lines = {
            "1163751742",
            "1381373672",
            "2136511328",
            "3694931569",
            "7463417111",
            "1319128137",
            "1359912421",
            "3125421639",
            "1293138521",
            "2311944581"
        };

        if (PartOne(lines) != "40") return false;
        if (PartTwo(lines) != "315") return false;
        
        lines = {
            "19111",
            "11191",
            "99991"
        };
        */
        std::vector<std::string> lines = {
            "19111",
            "11191",
            "99991"
        };
        if (Solve(ParseInput(lines)) != 8) return false;

        lines = {
            "119",
            "919",
            "119",
            "199",
            "111"
        };
        if (Solve(ParseInput(lines)) != 8) return false;

        return true;
    }
}