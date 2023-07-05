#include "2021/d15_Chiton.h"
#include "Algorithms/AStar.h"

SOLUTION(2021, 15) {
    using Map = std::vector<std::vector<u32>>;
    constexpr Map ParseInput(const std::vector<std::string>&lines) {
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

    constexpr size_t CalculatePathCost(const Map & map, const std::vector<RowCol>&path) {
        size_t result = 0;
        for (size_t i = 1; i < path.size(); i++) {
            auto pos = path[i];
            result += map[pos.Row][pos.Col];
        }

        return result;
    }

    size_t FindShortestPath(const Map & map) {
        RowCol start = { 0, 0 };
        RowCol end = { map.size() - 1, map[0].size() - 1 };

        auto cost = [&](const RowCol&, const RowCol& to) {
            return map[to.Row][to.Col];
        };
        auto isComplete = [end](const RowCol& pos) {
            return pos == end;
        };
        auto h = [end](const RowCol& pos) {
            return MDistance(pos, end);
        };
        auto neighbors = [&](const RowCol& pos) {
            return GetDirectNeighbors(pos, end);
        };
        auto path = AStarMin<RowCol>(start, cost, isComplete, h, neighbors);
        return CalculatePathCost(map, path);
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto map = ParseInput(lines);
        return FindShortestPath(map);
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

    auto Part2(const std::vector<std::string>&lines) {
        auto map = ParseInput(lines);
        auto bigMap = ExpandMap(map);
        return FindShortestPath(bigMap);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
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

        if (Part1(lines) != 40) return false;
        if (Part2(lines) != 315) return false;
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