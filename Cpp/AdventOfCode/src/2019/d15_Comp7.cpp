#include "2019/d15_Comp7.h"
#include "2019/Comp.h"

#include "Core/Algorithms/AStar.h"
#include "Core/Algorithms/FloodFill.h"

SOLUTION(2019, 15) {
    enum Dir { North = 1, South = 2, West = 3, East = 4 };
    enum Status { Wall = 0, Moved = 1, Reached = 2 };

    using Map = Constexpr::SmallMap<Coord, Status>;

    constexpr Status MoveBot(std::vector<s64>&instructions, Args & args, Dir dir) {
        args.Inputs.push_back(dir);
        args.Output = Unset;

        while (Apply(instructions, args)) {
            if (args.Output != Unset) {
                return static_cast<Status>(args.Output);
            }
        }

        return Wall; //should never happen
    }

    constexpr void Recurse(std::vector<s64> instructions, Args args, Map & map, Coord pos, Dir dir) {
        if (map.contains(pos)) return;

        map[pos] = MoveBot(instructions, args, dir);
        if (map.at(pos) == Wall) {
            return;
        }

        Coord north = { pos.X, pos.Y - 1 };
        Coord south = { pos.X, pos.Y + 1 };
        Coord east = { pos.X - 1, pos.Y };
        Coord west = { pos.X + 1, pos.Y };

        if (!map.contains(north)) Recurse(instructions, args, map, north, North);
        if (!map.contains(south)) Recurse(instructions, args, map, south, South);
        if (!map.contains(east)) Recurse(instructions, args, map, east, East);
        if (!map.contains(west)) Recurse(instructions, args, map, west, West);
    }

    constexpr Map Explore(std::vector<s64>&instructions) {
        Map map;
        Coord north = { 0, -1 };
        Coord south = { 0, 1 };
        Coord east = { -1, 0 };
        Coord west = { 1, 0 };
        Args args;
        Recurse(instructions, args, map, north, North);
        Recurse(instructions, args, map, south, South);
        Recurse(instructions, args, map, east, East);
        Recurse(instructions, args, map, west, West);

        return map;
    }

    constexpr Coord GetTarget(const Map & map) {
        Coord target{ 0, 0 };
        for (const auto& kvp : map) {
            if (kvp.second == Reached) {
                target = kvp.first;
                break;
            }
        }

        return target;
    }

    constexpr std::vector<Coord> GetNeighbors(const Map & map, Coord pos, Coord min, Coord max) {
        std::vector<Coord> result;
        auto neighbors = GetDirectNeighbors(pos, max, min);
        std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](const Coord& n) {
            return map.at(n) != Wall;
            });
        return result;
    }

    constexpr u32 GetShortestPath(const Map & map, Coord target) {
        Coord min, max;
        GetLimitsFromMap(map, min, max);
        Coord start = { 0, 0 };

        auto neighbors = [&](const Coord& pos) {
            return GetNeighbors(map, pos, min, max);
        };
        return static_cast<u32>(AStarMin<Coord>(start, target, neighbors).size() - 1);
    }

    constexpr u32 FloodFill(const Map & map, Coord start) {
        u32 result = 0;
        Constexpr::SmallSet<Coord> seen;
        std::vector<Coord> current;
        std::vector<Coord> next;
        current.push_back(start);

        Coord min, max;
        GetLimitsFromMap(map, min, max);

        while (!current.empty()) {
            result++;
            next.clear();
            for (const auto& pos : current) {
                seen.insert(pos);
                auto neighbors = GetNeighbors(map, pos, min, max);
                for (const auto& neighbor : neighbors) {
                    if (!seen.contains(neighbor) && std::find(next.begin(), next.end(), neighbor) == next.end()) {
                        next.push_back(neighbor);
                    }
                }
            }
            current = next;
        }

        return result - 1;
    }

    PART_ONE() {
        auto instructions = ParseInstructions(lines[0]);
        auto map = Explore(instructions);
        auto target = GetTarget(map);

        return Constexpr::ToString(GetShortestPath(map, target));
    }

    PART_TWO() {
        auto instructions = ParseInstructions(lines[0]);
        auto map = Explore(instructions);
        auto target = GetTarget(map);

        return Constexpr::ToString(FloodFill(map, target));
    }

    TESTS() {
        return true;
    }
}