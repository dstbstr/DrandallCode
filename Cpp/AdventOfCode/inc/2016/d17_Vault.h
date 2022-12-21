#pragma once

#include "Common.h"

#include "md5Constexpr.h"

enum struct Direction {Up, Down, Left, Right};
constexpr bool IsOpen(char value) {
    return value > 'a';
    //return value > 'a' && value < 'g';
}
static_assert(IsOpen('b'));
static_assert(IsOpen('f'));
static_assert(!IsOpen('0'));
static_assert(!IsOpen('a'));
static_assert(!IsOpen('9'));

constexpr std::vector<char> GetOpenDoors(const std::string& hash) {
    std::vector<char> result;
    if (IsOpen(hash[0])) result.push_back('U');
    if (IsOpen(hash[1])) result.push_back('D');
    if (IsOpen(hash[2])) result.push_back('L');
    if (IsOpen(hash[3])) result.push_back('R');

    return result;
}

static_assert(GetOpenDoors("ced9") == std::vector<char>{'U', 'D', 'L'});

constexpr std::vector<char> GetValidDirections(size_t pos) {
    size_t row, col;
    GetRowCol<4, 4>(pos, row, col);
    
    std::vector<char> result;
    if (row > 0) {
        result.push_back('U');
    }
    if (row < 3) {
        result.push_back('D');
    }
    if (col > 0) {
        result.push_back('L');
    }
    if (col < 3) {
        result.push_back('R');
    }

    return result;
}

static_assert(GetValidDirections(0) == std::vector<char>{'D', 'R'});

constexpr std::vector<char> GetNextDirections(size_t pos, const std::string& hash) {
    auto openDoors = GetOpenDoors(hash);
    auto validDirections = GetValidDirections(pos);
    std::vector<char> result;
    std::copy_if(openDoors.cbegin(), openDoors.cend(), std::back_inserter(result), [&validDirections](char door) {
        return std::find(validDirections.cbegin(), validDirections.cend(), door) != validDirections.cend();
        });

    return result;
}

constexpr size_t GetNewPos(size_t pos, char dir) {
    size_t row, col;
    GetRowCol<4, 4>(pos, row, col);

    switch (dir) {
    case 'U': return GetIndex<4, 4>(row - 1, col);
    case 'D': return GetIndex<4, 4>(row + 1, col);
    case 'L': return GetIndex<4, 4>(row, col - 1);
    case 'R': return GetIndex<4, 4>(row, col + 1);
    }

    return pos;
}

static_assert(GetNewPos(0, 'R') == 1);
static_assert(GetNewPos(0, 'D') == 4);

constexpr std::string FindShortestPath(const std::string& key) {
    constexpr size_t finalIndex = GetIndex<4, 4>(3, 3);

    std::vector<std::pair<size_t, std::string>> current{ {0, key } };
    std::vector<std::pair<size_t, std::string>> next;

    while (true) {
        for (auto [pos, path] : current) {
            if (pos == finalIndex) {
                return path.substr(key.size());
            }
            auto hash = ToHexLower(md5::compute(path.c_str()));
            auto validNextMoves = GetNextDirections(pos, hash);
            for (auto move : validNextMoves) {
                next.push_back(std::make_pair(GetNewPos(pos, move), path + move));
            }
        }

        current = next;
        next.clear();

        if (current.empty()) {
            break; //fail case
        }
    }

    return key;
}

constexpr u32 FindLongestPath(const std::string& key) {
    constexpr size_t finalIndex = GetIndex<4, 4>(3, 3);

    u32 pathLength = 0;
    u32 best = 0;
    std::vector<std::pair<size_t, std::string>> current{ {0, key} };
    std::vector<std::pair<size_t, std::string>> next;

    while (true) {
        for (auto [pos, path] : current) {
            if (pos == finalIndex) {
                best = pathLength;
                continue;
            }
            auto hash = ToHexLower(md5::compute(path.c_str()));
            auto validNextMoves = GetNextDirections(pos, hash);
            for (auto move : validNextMoves) {
                next.push_back(std::make_pair(GetNewPos(pos, move), path + move));
            }
        }

        current = next;
        next.clear();
        if (current.empty()) {
            return best; //success
        }
        else {
            pathLength++;
        }
    }
}

static_assert(FindShortestPath("hijkl") == "hijkl");
static_assert(FindShortestPath("ihgpwlah") == "DDRRRD");
//static_assert(FindShortestPath("kglvqrro") == "DDUDRLRRUDRD");
//static_assert(FindShortestPath("ulqzkmiv") == "DRURDRUDDLLDLUURRDULRLDUUDDDRR");


std::string Run(const std::vector<std::string>&) {
    //return FindShortestPath("bwnlcvfs");
    return ToString(FindLongestPath("bwnlcvfs"));
}

bool RunTests() {
    //if (FindShortestPath("hijkl") != "hijkl") return false;
    //if (FindShortestPath("ihgpwlah") != "DDRRRD") return false;
    //if (FindShortestPath("kglvqrro") != "DDUDRLRRUDRD") return false;
    //if (FindShortestPath("ulqzkmiv") != "DRURDRUDDLLDLUURRDULRLDUUDDDRR") return false;

    if (FindLongestPath("ihgpwlah") != 370) return false;
    if (FindLongestPath("kglvqrro") != 492) return false;
    if (FindLongestPath("ulqzkmiv") != 830) return false;
    return true;
}