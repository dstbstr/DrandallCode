#include "2018/d22_Cave.h"
#include "Algorithms/AStar.h"

SOLUTION(2018, 22) {
    static constexpr u64 limit = 20183;
    enum struct GroundType { Rocky, Wet, Narrow };
    enum struct Equipment { None, Climbing, Torch };

    using Map = std::vector<std::vector<u64>>;
    using GroundMap = std::vector<std::vector<GroundType>>;

    constexpr u64 GetIndex(size_t row, size_t col, RowCol target, const Map & map, const std::vector<u64>&currentLine) {
        if (row == 0 && col == 0) return 0;
        else if (row == target.Row && col == target.Col) return 0;
        else if (row == 0) return col * 16807;
        else if (col == 0) return row * 48271;
        else return map[row - 1][col] * currentLine.back();
    }

    constexpr Map BuildMap(RowCol target, RowCol limits, u32 depth) {
        Map result;
        for (size_t row = 0; row <= limits.Row; row++) {
            std::vector<u64> line;
            for (size_t col = 0; col <= limits.Col; col++) {
                auto index = GetIndex(row, col, target, result, line);
                line.push_back((index + depth) % limit);
            }
            result.push_back(line);
        }
        return result;
    }

    GroundMap BuildGroundMap(const Map & map) {
        GroundMap result;
        for (size_t row = 0; row < map.size(); row++) {
            std::vector<GroundType> line;
            for (size_t col = 0; col < map[row].size(); col++) {
                auto val = map[row][col] % 3;
                line.push_back(val == 0 ? GroundType::Rocky : val == 1 ? GroundType::Wet : GroundType::Narrow);
            }
            result.push_back(line);
        }

        return result;
    }

    void PrintMap(GroundMap & map, RowCol target) {
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = 0; col < map[row].size(); col++) {
                auto val = map[row][col];
                char icon = val == GroundType::Rocky ? '.' : val == GroundType::Wet ? '=' : '|';
                if (row == 0 && col == 0) icon = 'M';
                else if (row == target.Row && col == target.Col) icon = 'T';
                std::cout << icon;
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    constexpr u32 GetRisk(GroundMap & map) {
        u32 result = 0;
        for (size_t row = 0; row < map.size(); row++) {
            for (size_t col = 0; col < map[row].size(); col++) {
                auto val = map[row][col];
                result += (val == GroundType::Rocky ? 0 : val == GroundType::Wet ? 1 : 2);
            }
        }
        return result;
    }

    auto Part1(RowCol target, u32 depth) {
        auto map = BuildGroundMap(BuildMap(target, target, depth));
        //PrintMap(map, target);
        return GetRisk(map);
    }

    constexpr bool CanMove(GroundType groundType, Equipment held) {
        switch (held) {
        case Equipment::None: return groundType != GroundType::Rocky;
        case Equipment::Torch: return groundType != GroundType::Wet;
        case Equipment::Climbing: return groundType != GroundType::Narrow;
        }
        return true;
    }

    constexpr std::vector<Equipment> FindCommon(GroundType lhs, GroundType rhs) {
        std::vector<Equipment> result;

        if (CanMove(lhs, Equipment::None) && CanMove(rhs, Equipment::None)) result.push_back(Equipment::None);
        if (CanMove(lhs, Equipment::Torch) && CanMove(rhs, Equipment::Torch)) result.push_back(Equipment::Torch);
        if (CanMove(lhs, Equipment::Climbing) && CanMove(rhs, Equipment::Climbing)) result.push_back(Equipment::Climbing);

        return result;
    }

    u32 FindShortestPath(const GroundMap & map, RowCol target) {
        struct Santa {
            RowCol Pos;
            Equipment Held;
            constexpr bool operator==(const Santa& other) const {
                return Pos == other.Pos && Held == other.Held;
            }
        };
        struct SantaHash {
            size_t operator()(const Santa& s) const {
                return s.Pos.Row ^ s.Pos.Col + static_cast<size_t>(s.Held);
            }
        };
        RowCol limits = { map.size() - 1, map[0].size() - 1 };
        Santa start = { { 0, 0 }, Equipment::Torch };
        Santa end = { target, Equipment::Torch };

        auto costFunc = [&](const Santa& from, const Santa& to) {
            return from.Held == to.Held ? 1 : 8;
        };
        auto isCompleteFunc = [end](const Santa& santa) {
            return santa == end;
        };
        auto h = [target](const Santa& santa) {
            u32 result = santa.Held == Equipment::Torch ? 0 : 7;
            return result + static_cast<u32>(MDistance(santa.Pos, target));
        };
        auto neighborFunc = [&](const Santa& santa) -> std::vector<Santa> {
            auto neighbors = GetDirectNeighbors(santa.Pos, limits);

            std::vector<Santa> result;
            for (const auto& neighbor : neighbors) {
                auto currentGround = map[santa.Pos.Row][santa.Pos.Col];
                auto nextGround = map[neighbor.Row][neighbor.Col];
                for (const auto& equipment : FindCommon(currentGround, nextGround)) {
                    result.push_back({ neighbor, equipment });
                }
            }

            return result;
        };

        //auto path = AStarMin<Santa, SantaHash>(start, costFunc, isCompleteFunc, h, neighborFunc);
        auto path = AStarMin<Santa>(start, costFunc, isCompleteFunc, h, neighborFunc);
        u32 result = 0;
        Equipment previous = Equipment::Torch;
        for (const auto& step : path) {
            if (previous != step.Held) {
                result += 7;
                previous = step.Held;
            }
            result++;
        }

        return result - 1; //doesn't cost a step to start
    }

    //infinite loop?  10 minutes in release...
    auto Part2(RowCol target, u32 depth) {
        RowCol buffer = { target.Row + 50, target.Col + 500 };
        //RowCol buffer = { target.Row + 10, target.Col + 10 };
        auto map = BuildGroundMap(BuildMap(target, buffer, depth));

        return FindShortestPath(map, target);
    }

    std::string Run(const std::vector<std::string>&) {
        RowCol target = { 760, 14 };
        //return Constexpr::ToString(Part1(target, 7863));
        return Constexpr::ToString(Part2(target, 7863));
    }

    bool RunTests() {
        RowCol target = { 10, 10 };
        //if (Part1(target, 510) != 114) return false;
        if (Part2(target, 510) != 45) return false;
        return true;
    }
    //depth: 7863
    //target: 14, 760

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