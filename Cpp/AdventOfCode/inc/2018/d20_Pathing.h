#pragma once

#include "Common.h"

#include <iostream>

using Grid = std::unordered_map<Coord, std::vector<Coord>>;

void Connect(Coord& a, const Coord& b, Grid& grid) {
    grid[a].push_back(b);
    grid[b].push_back(a);
    a = b;
}

void Recurse(std::string_view chars, size_t& index, Coord pos, Grid& grid) {
    auto origin = pos;
    auto& [x, y] = pos;
    while(true) {
        switch (chars[index++]) {
        case 'N': Connect(pos, { x, y - 1 }, grid); break;
        case 'S': Connect(pos, { x, y + 1 }, grid); break;
        case 'E': Connect(pos, { x - 1, y }, grid); break;
        case 'W': Connect(pos, { x + 1, y }, grid); break;
        case '(': Recurse(chars, index, pos, grid); break;
        case ')': return;
        case '|': pos = origin; break;
        case '$': return;
        }
    }
}

auto PartOne(const std::string& line) {
    size_t index = 1;
    Grid grid{};
    Recurse(line, index, { 0, 0 }, grid);

    std::unordered_set<Coord> visited;
    std::queue<std::pair<Coord, int>> queue;
    queue.push({ {0, 0}, 0 });
    auto longest = 0;
    while (!queue.empty()) {
        auto [pos, length] = queue.front();
        queue.pop();
        if (visited.contains(pos)) continue;
        visited.insert(pos);
        longest = std::max(longest, length);
        for (auto neighbors : grid[pos]) {
            queue.push({ neighbors, length + 1 });
        }
    }

    return longest;
}

auto PartTwo(const std::string& line) {
    size_t index = 1;
    Grid grid{};
    Recurse(line, index, { 0, 0 }, grid);

    std::unordered_set<Coord> visited;
    std::queue<std::pair<Coord, int>> queue;
    queue.push({ {0, 0}, 0 });
    auto overTarget = 0;
    while (!queue.empty()) {
        auto [pos, length] = queue.front();
        queue.pop();
        if (visited.contains(pos)) continue;
        visited.insert(pos);
        if (length >= 1000) overTarget++;
        for (auto neighbors : grid[pos]) {
            queue.push({ neighbors, length + 1 });
        }
    }

    return overTarget;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    /*
    auto paths = GetSuffixes(regex);
    if (std::find(paths.begin(), paths.end(), "ENNWSWWSSSEENEENNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWNEWSSSSEENEENNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWSSSEENWNSEEENNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWSSSEENEESWENNNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWNEWSSSSEENWNSEEENNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWNEWSSSSEENEESWENNNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWSSSEENWNSEEESWENNNN") == paths.end()) return false;
    if (std::find(paths.begin(), paths.end(), "ENNWSWWNEWSSSSEENWNSEEESWENNNN") == paths.end()) return false;
    if (paths.size() != 8) return false;
    */
    if (PartOne("^WNE$") != 3) return false;
    if (PartOne("^ENWWW(NEEE|SSE(EE|N))$") != 10) return false;
    if (PartOne("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$") != 18) return false;
    if (PartOne("^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$") != 23) return false;
    if (PartOne("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$") != 31) return false;
    return true;
}