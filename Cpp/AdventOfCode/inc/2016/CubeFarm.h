#pragma once

#include "Common.h"

#include <bit>

constexpr bool IsOpen(UCoord coord, u32 key) {
    u32 val = (coord.X * coord.X) + (3 * coord.X) + (coord.X * coord.Y * 2) + coord.Y + (coord.Y * coord.Y) + key;
    return std::popcount(val) % 2 == 0;
}

static_assert(IsOpen({ 0, 0 }, 10));
static_assert(!IsOpen({ 1, 0 }, 10));
static_assert(IsOpen({2, 0}, 10));
static_assert(IsOpen({0, 1}, 10));
static_assert(IsOpen({ 1, 1 }, 10));

constexpr u32 GetDistance(UCoord location, UCoord target) {
    auto x = 0;
    auto y = 0;
    if (location.X > target.X) {
        x = location.X - target.X;
    }
    else {
        x = target.X - location.X;
    }
    if (location.Y > target.Y) {
        y = location.Y - target.Y;
    }
    else {
        y = target.Y - location.Y;
    }
    return x + y;
}

void AddNextMoves(UCoord location, std::vector<UCoord>& next) {
    if (location.X > 0) {
        next.push_back({ location.X - 1, location.Y });
    }
    if (location.Y > 0) {
        next.push_back({ location.X, location.Y - 1 });
    }
    next.push_back({ location.X + 1, location.Y });
    next.push_back({ location.X, location.Y + 1 });
}

u32 Bfs(u32 key, UCoord target) {
    std::unordered_set<UCoord, UCoordHash> seen;
    u32 depth = 0;
    std::vector<UCoord> moves;
    moves.push_back({ 1, 1 });

    while (true) {
        if (std::find(moves.cbegin(), moves.cend(), target) != moves.cend()) {
            return depth;
        }
        depth++;
        std::vector<UCoord> next;
        for (auto move : moves) {
            AddNextMoves(move, next);
        }
        moves.clear();
        std::copy_if(next.begin(), next.end(), std::back_inserter(moves), [&](UCoord move) {
            return seen.find(move) == seen.end() && IsOpen(move, key);
            });

        seen.insert(moves.begin(), moves.end());

        if (moves.size() == 0) {
            return 0; //error case, out of moves
        }
    }
    return 0;
}

u32 FindUniquePositions(u32 key, u32 maxSteps) {
    std::unordered_set<UCoord, UCoordHash> seen;
    u32 depth = 0;
    std::vector<UCoord> moves;
    moves.push_back({ 1, 1 });
    seen.insert({ 1, 1 });
    while (depth < maxSteps) {
        depth++;
        std::vector<UCoord> next;
        for (auto move : moves) {
            AddNextMoves(move, next);
        }
        moves.clear();
        std::copy_if(next.begin(), next.end(), std::back_inserter(moves), [&](UCoord move) {
            return seen.find(move) == seen.end() && IsOpen(move, key);
            });

        seen.insert(moves.begin(), moves.end());

        if (moves.size() == 0) {
            return 0; //error case, out of moves
        }
    }
    return static_cast<u32>(seen.size());

}

std::string Run(const std::vector<std::string>&) {
    //return ToString(Bfs(1362, { 31, 39 }));
    return ToString(FindUniquePositions(1362, 50));
}

bool RunTests() {
    if (Bfs(10, { 7, 4 }) != 11) return false;

    return true;
}