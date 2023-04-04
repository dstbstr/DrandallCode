#pragma once

#include "Common.h"

enum struct HexDir {East, West, SouthEast, SouthWest, NorthEast, NorthWest};

template<typename T>
constexpr void Move(Vec3<T>& pos, HexDir dir) {
    switch (dir) {
    case HexDir::East: pos.X++; pos.Z--; return;
    case HexDir::West: pos.X--; pos.Z++; return;
    case HexDir::SouthEast: pos.Y++; pos.Z--; return;
    case HexDir::SouthWest: pos.X--; pos.Y++; return;
    case HexDir::NorthEast: pos.X++; pos.Y--; return;
    case HexDir::NorthWest: pos.Y--; pos.Z++; return;
    }
}

constexpr std::vector<HexDir> GetDirections(const std::string& line) {
    std::vector<HexDir> result;
    for (auto i = 0; i < line.size(); i++) {
        auto c = line[i];
        if (c == 'e') result.push_back(HexDir::East);
        else if (c == 'w') result.push_back(HexDir::West);
        else {
            auto c2 = line[++i];
            if (c == 'n') {
                if (c2 == 'e') result.push_back(HexDir::NorthEast);
                else result.push_back(HexDir::NorthWest);
            }
            else {
                if (c2 == 'e') result.push_back(HexDir::SouthEast);
                else result.push_back(HexDir::SouthWest);
            }
        }
    }
    return result;
}

static_assert(GetDirections("esew") == std::vector<HexDir>{HexDir::East, HexDir::SouthEast, HexDir::West});

constexpr bool TestMove() {
    Vec3<s64> pos = { 0, 0, 0 };
    auto directions = GetDirections("nwwswee");
    for (auto dir : directions) {
        Move(pos, dir);
    }

    if (pos != Vec3<s64>{0, 0, 0}) return false;

    return true;
}

static_assert(TestMove());

using Grid = std::unordered_set<Vec3<s64>, Vec3Hash<s64>>;

Grid GetInitialGrid(const std::vector<std::string>& lines) {
    Grid result{};
    const Vec3<s64> origin = { 0, 0, 0 };
    for (const auto& line : lines) {
        auto directions = GetDirections(line);
        auto pos = origin;
        for (auto dir : directions) {
            Move(pos, dir);
        }
        if (result.contains(pos)) {
            result.erase(pos);
        }
        else {
            result.insert(pos);
        }
    }

    return result;
}

auto PartOne(const std::vector<std::string>& lines) {
    auto grid = GetInitialGrid(lines);
    return grid.size();
}

template<typename T>
constexpr std::vector<Vec3<T>> FindNeighbors(const Vec3<T>& pos) {
    std::vector<Vec3<T>> result = { pos, pos, pos, pos, pos, pos };
    Move(result[0], HexDir::East);
    Move(result[1], HexDir::West);
    Move(result[2], HexDir::NorthEast);
    Move(result[3], HexDir::NorthWest);
    Move(result[4], HexDir::SouthEast);
    Move(result[5], HexDir::SouthWest);

    return result;
}


void NextDay(Grid& grid) {
    Grid seen;
    std::vector<Vec3<s64>> toCheck;
    for (const auto& pos : grid) {
        toCheck.push_back(pos);
    }

    Grid next;
    while (!toCheck.empty()) {
        auto current = toCheck.back();
        toCheck.pop_back();
        auto neighbors = FindNeighbors(current);
        auto countMarked = std::count_if(neighbors.begin(), neighbors.end(), [&grid](const Vec3<s64>& p) {
            return grid.contains(p);
            });
        if (countMarked == 0) continue;

        if (grid.contains(current)) {
            if (countMarked < 3) {
                next.insert(current);
            }
        }
        else {
            if (countMarked == 2) {
                next.insert(current);
            }
        }

        for (const auto& neighbor : neighbors) {
            if (!seen.contains(neighbor)) {
                seen.insert(neighbor);
                toCheck.push_back(neighbor);
            }
        }
    }
    grid = next;
}

auto PartTwo(const std::vector<std::string>& lines, size_t days) {
    auto grid = GetInitialGrid(lines);
    for (auto i = 0; i < days; i++) {
        NextDay(grid);
    }

    return grid.size();
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines, 100));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "sesenwnenenewseeswwswswwnenewsewsw",
        "neeenesenwnwwswnenewnwwsewnenwseswesw",
        "seswneswswsenwwnwse",
        "nwnwneseeswswnenewneswwnewseswneseene",
        "swweswneswnenwsewnwneneseenw",
        "eesenwseswswnenwswnwnwsewwnwsene",
        "sewnenenenesenwsewnenwwwse",
        "wenwwweseeeweswwwnwwe",
        "wsweesenenewnwwnwsenewsenwwsesesenwne",
        "neeswseenwwswnwswswnw",
        "nenwswwsewswnenenewsenwsenwnesesenew",
        "enewnwewneswsewnwswenweswnenwsenwsw",
        "sweneswneswneneenwnewenewwneswswnese",
        "swwesenesewenwneswnwwneseswwne",
        "enesenwswwswneneswsenwnewswseenwsese",
        "wnwnesenesenenwwnenwsewesewsesesew",
        "nenewswnwewswnenesenwnesewesw",
        "eneswnwswnwsenenwnwnwwseeswneewsenese",
        "neswnwewnwnwseenwseesewsenwsweewe",
        "wseweeenwnesenwwwswnew"
    };

    if (PartOne(lines) != 10) return false;
    if (PartTwo(lines, 1) != 15) return false;
    if (PartTwo(lines, 100) != 2208) return false;
    return true;
}