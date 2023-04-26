#pragma once

#include "Common.h"
#include "AStar.h"

using Walls = std::vector<std::vector<bool>>;

Walls FindWalls(const std::vector<std::string>& lines) {
    Walls result;
    for (auto row = 0; row < lines.size(); row++) {
        std::vector<bool> line;
        for (auto col = 0; col < lines[row].size(); col++) {
            line.push_back(lines[row][col] == '#');
        }
        result.push_back(line);
    }

    return result;
}

std::vector<RowCol> FindTargets(const std::vector<std::string>& lines) {
    std::vector<RowCol> result;
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            //if (lines[row][col] != '.' && lines[row][col] != '#') {
            if(lines[row][col] > '0') {
                result.push_back({ row, col });
            }
        }
    }
    
    return result;
}

RowCol FindStart(const std::vector<std::string>& lines) {
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            if (lines[row][col] == '0') {
                return RowCol{ row, col };
            }
        }
    }

    return {};
}

u32 FindPathLength(const std::vector<RowCol> targets, const Walls& walls, const RowCol& start) {
    static std::unordered_map<RowCol, std::unordered_map<RowCol, u32>> cache;

    RowCol currentTarget = { 0, 0 };
    RowCol limits = { walls.size(), walls[0].size() };

    auto costFunc = [](const RowCol& from, const RowCol& to) {
        return static_cast<u32>(MDistance(from, to));
    };
    auto compareFunc = [](u32 lhs, u32 rhs) {
        return lhs <= rhs;
    };
    auto isCompleteFunc = [&currentTarget](const RowCol& pos) {
        return pos == currentTarget;
    };
    auto heuristicFunc = [&currentTarget](const RowCol& pos) {
        return static_cast<u32>(MDistance(pos, currentTarget));
    };
    auto neighborFunc = [&walls, &limits](const RowCol& pos) {
        auto neighbors = GetDirectNeighbors(pos, limits);
        std::vector<RowCol> result;
        std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&walls](const RowCol& rc) {
            return !walls[rc.Row][rc.Col];
            });
        return result;
    };

    auto begin = start;
    u32 result = 0;
    for (const auto& target : targets) {
        currentTarget = target;
        if (cache.find(begin) == cache.end() || cache.at(begin).find(currentTarget) == cache.at(begin).end()) {
            auto path = AStar<RowCol>(begin, costFunc, compareFunc, isCompleteFunc, heuristicFunc, neighborFunc);
            cache[begin][currentTarget] = static_cast<u32>(path.size() - 1);
        }
        result += cache.at(begin).at(currentTarget);
        begin = currentTarget;
    }

    return result;
}

u32 PartOne(const std::vector<std::string>& lines) {
    auto walls = FindWalls(lines);
    auto targets = FindTargets(lines);
    auto start = FindStart(lines);

    u32 result = 9999;
    std::sort(targets.begin(), targets.end());
    while (std::next_permutation(targets.begin(), targets.end())) {
        result = std::min(result, FindPathLength(targets, walls, start));
    }
    
    return result;
}

u32 PartTwo(const std::vector<std::string>& lines) {
    auto walls = FindWalls(lines);
    auto targets = FindTargets(lines);
    auto start = FindStart(lines);

    u32 result = 9999;
    std::sort(targets.begin(), targets.end());
    while (std::next_permutation(targets.begin(), targets.end())) {
        auto extendedTargets = targets;
        extendedTargets.push_back(start);
        result = std::min(result, FindPathLength(extendedTargets, walls, start));
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "###########",
        "#0.1.....2#",
        "#.#######.#",
        "#4.......3#",
        "###########"
    };
    if (PartOne(lines) != 14) return false;

    return true;
}