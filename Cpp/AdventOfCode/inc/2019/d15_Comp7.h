#pragma once

#include "Common.h"
#include "Comp.h"
#include "AStar.h"

enum Dir {North = 1, South = 2, West = 3, East = 4};
enum Status {Wall = 0, Moved = 1, Reached = 2};

using Map = std::unordered_map<Coord, Status, CoordHash>;

constexpr s64 Consumed = -9919;

Status MoveBot(std::vector<s64>& instructions, Args& args, Dir dir) {
    args.Inputs.push_back(dir);
    args.Output = Consumed;

    while (Apply(instructions, args)) {
        if (args.Output != Consumed) {
            return static_cast<Status>(args.Output);
        }
    }

    return Wall; //should never happen
}

void Recurse(std::vector<s64> instructions, Args args, Map& map, Coord pos, Dir dir) {
    if (map.find(pos) != map.end()) return;

    map[pos] = MoveBot(instructions, args, dir);
    if (map.at(pos) == Wall) {
        return;
    }

    Coord north = { pos.X, pos.Y - 1 };
    Coord south = { pos.X, pos.Y + 1 };
    Coord east = { pos.X - 1, pos.Y };
    Coord west = { pos.X + 1, pos.Y };

    if (map.find(north) == map.end()) Recurse(instructions, args, map, north, North);
    if (map.find(south) == map.end()) Recurse(instructions, args, map, south, South);
    if (map.find(east) == map.end()) Recurse(instructions, args, map, east, East);
    if (map.find(west) == map.end()) Recurse(instructions, args, map, west, West);
}

Map Explore(std::vector<s64>& instructions) {
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

Coord GetTarget(const Map& map) {
    Coord target{ 0, 0 };
    for (const auto& kvp : map) {
        if (kvp.second == Reached) {
            target = kvp.first;
            break;
        }
    }

    return target;
}

void PrintMap(const Map& map) {
    auto toStr = [](const Coord& pos, const Status& status) {
        static Coord origin = { 0, 0 };
        if (pos == origin) return 'S';
        return status == Wall ? '#' : status == Moved ? '.' : 'T';
    };

    PrintMap<Coord, Status, CoordHash>(map, toStr, ' ');
}

std::vector<Coord> GetNeighbors(const Map& map, Coord pos, Coord min, Coord max) {
    std::vector<Coord> result;
    auto neighbors = GetDirectNeighbors(pos, max, min);
    std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](const Coord& n) {
        return map.at(n) != Wall;
        });
    return result;
}

u32 GetShortestPath(const Map& map, Coord target) {
    Coord min, max;
    GetLimitsFromMap(map, min, max);

    Coord start = { 0, 0 };
    auto costFunc = [](const Coord&, const Coord&) {
        return 1;
    };
    auto isCompleteFunc = [target](const Coord& pos) {
        return pos == target;
    };
    auto h = [target](const Coord& pos) {
        return static_cast<u32>(MDistance(target, pos));
    };
    auto neighbors = [&](const Coord& pos) {
        return GetNeighbors(map, pos, min, max);
    };
    return static_cast<u32>(AStarMin<Coord, CoordHash>(start, costFunc, isCompleteFunc, h, neighbors).size() - 1);
}

auto PartOne(const std::string& line) {
    auto instructions = ParseInstructions(line);
    auto map = Explore(instructions);
    auto target = GetTarget(map);

    PrintMap(map);

    return GetShortestPath(map, target);
}

u32 FloodFill(const Map& map, Coord start) {
    u32 result = 0;
    std::unordered_set<Coord, CoordHash> seen;
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

auto PartTwo(const std::string& line) {
    auto instructions = ParseInstructions(line);
    auto map = Explore(instructions);
    auto target = GetTarget(map);

    return FloodFill(map, target);
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    return true;
}