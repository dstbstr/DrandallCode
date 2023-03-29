#pragma once

#include "Common.h"

using Reactor = std::unordered_set<Vec3<s32>, Vec3Hash<s32>>;
using BigReactor = std::unordered_set<Vec4<s32>, Vec4Hash<s32>>;

Reactor ParseInput(const std::vector<std::string>& lines) {
    Reactor result;
    for (s32 row = 0; row < lines.size(); row++) {
        for (s32 col = 0; col < lines[row].size(); col++) {
            if (lines[row][col] == '#') {
                result.insert({ col, row, 0 });
            }
        }
    }

    return result;
}

BigReactor ParseInput2(const std::vector<std::string>& lines) {
    BigReactor result;
    for (s32 row = 0; row < lines.size(); row++) {
        for (s32 col = 0; col < lines[row].size(); col++) {
            if (lines[row][col] == '#') {
                result.insert({ col, row, 0, 0 });
            }
        }
    }

    return result;
}

bool ShouldActivate(Vec3<s32> pos, const Reactor& current) {
    auto neighbors = GetAllNeighbors(pos);
    auto activeNeighbors = std::count_if(neighbors.cbegin(), neighbors.cend(), [&current](Vec3<s32> neighbor) {
        return current.contains(neighbor);
        });

    if (current.contains(pos)) {
        return activeNeighbors == 2 || activeNeighbors == 3;
    }
    else {
        return activeNeighbors == 3;
    }
}

bool ShouldActivate(Vec4<s32> pos, const BigReactor& current) {
    auto neighbors = GetAllNeighbors(pos);
    auto activeNeighbors = std::count_if(neighbors.cbegin(), neighbors.cend(), [&current](Vec4<s32> neighbor) {
        return current.contains(neighbor);
        });

    if (current.contains(pos)) {
        return activeNeighbors == 2 || activeNeighbors == 3;
    }
    else {
        return activeNeighbors == 3;
    }
}

void ForEach(const Reactor& reactor, std::function<void(const Vec3<s32>&)> func) {
    Vec3<s32> min, max;
    GetLimits(reactor, min, max);
    for (s32 x = min.X - 1; x <= max.X + 1; x++) {
        for (s32 y = min.Y - 1; y <= max.Y + 1; y++) {
            for (s32 z = min.Z - 1; z <= max.Z + 1; z++) {
                Vec3<s32> nextPos = { x, y, z };
                func(nextPos);
            }
        }
    }
}

void ForEach(const BigReactor& reactor, std::function<void(const Vec4<s32>&)> func) {
    Vec4<s32> min, max;
    GetLimits(reactor, min, max);
    for (s32 x = min.X - 1; x <= max.X + 1; x++) {
        for (s32 y = min.Y - 1; y <= max.Y + 1; y++) {
            for (s32 z = min.Z - 1; z <= max.Z + 1; z++) {
                for (s32 w = min.W - 1; w <= max.W + 1; w++) {
                    Vec4<s32> nextPos = { x, y, z, w };
                    func(nextPos);
                }
            }
        }
    }
}

Reactor Next(const Reactor& current) {
    Reactor next;
    ForEach(current, [&next, current](const Vec3<s32>& pos) {
        if (ShouldActivate(pos, current)) {
            next.insert(pos);
        }
        });
    return next;
}

BigReactor Next(const BigReactor& current) {
    BigReactor next;
    ForEach(current, [&next, current](const Vec4<s32>& pos) {
        if (ShouldActivate(pos, current)) {
            next.insert(pos);
        }
        });
    return next;
}

void PrintReactor(const Reactor& reactor) {
    Vec3<s32> min, max;
    GetLimits(reactor, min, max);
    for (s32 z = min.Z; z <= max.Z; z++) {
        std::cout << "z=" << z << "\n";
        for (s32 y = min.Y; y <= max.Y; y++) {
            for (s32 x = min.X; x <= max.X; x++) {
                Vec3<s32> pos = { x, y, z };

                std::cout << (reactor.contains(pos) ? '#' : '.');
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void PrintReactor(const BigReactor& reactor) {
    Vec4<s32> min, max;
    GetLimits(reactor, min, max);
    for (s32 w = min.W; w <= max.W; w++) {
        for (s32 z = min.Z; z <= max.Z; z++) {
            std::cout << "z=" << z << ", w=" << w << "\n";
            for (s32 y = min.Y; y <= max.Y; y++) {
                for (s32 x = min.X; x <= max.X; x++) {
                    Vec4<s32> pos = { x, y, z, w };

                    std::cout << (reactor.contains(pos) ? '#' : '.');
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }
    std::cout << "\n=========================\n\n";
}

auto PartOne(const std::vector<std::string>& lines) {
    auto reactor = ParseInput(lines);
    //PrintReactor(reactor);
    for (auto i = 0; i < 6; i++) {
        reactor = Next(reactor);
        //PrintReactor(reactor);
    }

    return reactor.size();
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto reactor = ParseInput2(lines);
    //PrintReactor(reactor);
    for (auto i = 0; i < 6; i++) {
        reactor = Next(reactor);
        //PrintReactor(reactor);
    }

    return reactor.size();
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        ".#.",
        "..#",
        "###"
    };

    if (PartOne(lines) != 112) return false;
    if (PartTwo(lines) != 848) return false;
    return true;
}