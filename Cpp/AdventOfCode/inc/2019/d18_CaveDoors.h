#pragma once

#include "Common.h"
#include "AStar.h"
#include <bitset>
#include <set>

using Map = std::unordered_map<RowCol, char, RowColHash>;
using DoorMap = std::unordered_map<char, RowCol>;

void ParseMap(const std::vector<std::string>& lines, Map& outMap, DoorMap& outDoorMap, u32& outAllKeys) {
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            RowCol rc = { row, col };
            auto c = lines[row][col];
            outMap[rc] = c;
            if (c != '#') {
                outDoorMap[c] = rc;
            }
            if (std::islower(c)) {
                Constexpr::SetBit(outAllKeys, c - 'a');
            }
        }
    }
}

size_t GetPathDistance(const Map& map, const RowCol& start, const RowCol& end, const u32 keys, const RowCol& limit) {
    auto n = [&](const RowCol& pos) {
        std::vector<RowCol> result;
        auto possible = Get4Neighbors(pos, limit);
        std::copy_if(possible.cbegin(), possible.cend(), std::back_inserter(result), [&](const RowCol& toPos) {
            auto c = map.at(toPos);
        if (c == '.' || c == '@' || std::islower(c)) return true;
        if (c == '#') return false;

        return Constexpr::IsBitSet(keys, c - 'A');
            });

        return result;
    };

    auto path = AStarMin<RowCol, RowColHash>(start, end, n);
    return path.empty() ? 0 : path.size() - 1;
}


auto PartOne(const std::vector<std::string>& lines) {
    RowCol limit = { lines.size() - 1, lines[0].size() - 1 };
    std::vector<RowCol> startPositions;
    u32 keys = 0;
    u32 allKeys = 0;
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            char c = lines[row][col];

            if (std::islower(c)) {
                Constexpr::SetBit(allKeys, c - 'a');
            }
            else if (c == '@') {
                startPositions.push_back({ row, col });
            }
        }
    }

    std::set<std::tuple<size_t, size_t, u32>> seen;

    struct State { RowCol Pos; u32 Steps; u32 Keys; };
    std::queue<State> queue;

    for (const auto& pos : startPositions) {
        queue.push({ pos, 0, 0 });
        seen.insert(std::make_tuple(pos.Row, pos.Col, 0));
    }

    while (!queue.empty()) {
        State current = queue.front();
        queue.pop();
        keys = current.Keys;

        if (keys == allKeys) {
            return current.Steps;
        }

        auto neighbors = Get4Neighbors(current.Pos, limit);
        for (const auto& neighbor : neighbors) {
            if (seen.contains(std::make_tuple(neighbor.Row, neighbor.Col, current.Keys))) continue;

            auto c = lines[neighbor.Row][neighbor.Col];
            if (c == '#') continue;
            if (std::isupper(c) && !Constexpr::IsBitSet(current.Keys, c - 'A')) continue;

            if (std::islower(c)) {
                Constexpr::SetBit(keys, c - 'a');
                seen.insert(std::make_tuple(neighbor.Row, neighbor.Col, keys));
                queue.push({ neighbor, current.Steps + 1, keys });
                Constexpr::UnsetBit(keys, c - 'a');
            }
            else {
                seen.insert(std::make_tuple(neighbor.Row, neighbor.Col, keys));
                queue.push({ neighbor, current.Steps + 1, keys });
            }
        }
    }

    return u32(0);
}


void SplitIntoRooms(std::vector<std::string>& lines) {
    RowCol origin{ 0, 0 };
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            if (lines[row][col] == '@') {
                origin = { row, col };
                break;
            }
        }
    }

    lines[origin.Row][origin.Col] = '#';
    lines[origin.Row - 1][origin.Col] = '#';
    lines[origin.Row + 1][origin.Col] = '#';
    lines[origin.Row][origin.Col - 1] = '#';
    lines[origin.Row][origin.Col + 1] = '#';
    lines[origin.Row - 1][origin.Col - 1] = '1';
    lines[origin.Row - 1][origin.Col + 1] = '2';
    lines[origin.Row + 1][origin.Col - 1] = '3';
    lines[origin.Row + 1][origin.Col + 1] = '4';
}

std::vector<char> FindReachableKeys(const Map& map, const DoorMap& doorMap, RowCol start, RowCol limit, u32 allKeys) {
    std::vector<char> result;
    for (auto key = 'a'; key <= 'z'; key++) {
        if (!doorMap.contains(key)) break;
        if (GetPathDistance(map, start, doorMap.at(key), allKeys, limit) > 0) {
            result.push_back(key);
        }
    }
    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    std::vector<std::string> lineCopy = lines;
    RowCol limit = { lines.size() - 1, lines[0].size() - 1 };
    SplitIntoRooms(lineCopy);
    Map map;
    DoorMap doorMap;
    u32 allKeys;
    ParseMap(lineCopy, map, doorMap, allKeys);

    std::vector<RowCol> startingPoints = { doorMap.at('1'), doorMap.at('2'), doorMap.at('3'), doorMap.at('4') };
    std::vector<std::vector<char>> reachableKeys;
    for (const auto& rc : startingPoints) {
        reachableKeys.push_back(FindReachableKeys(map, doorMap, rc, limit, allKeys));
    }

    struct State {
        std::vector<RowCol> BotPositions;
        u32 Keys;
        size_t Steps;
    };
    struct History {
        std::vector<RowCol> BotPositions;
        u32 Keys;

        constexpr bool operator==(const History& other) const {
            return BotPositions == other.BotPositions && Keys == other.Keys;
        }
    };
    struct HistoryHash {
        constexpr size_t operator()(const History& h) const {
            size_t result = h.Keys;
            for (const auto& pos : h.BotPositions) {
                result ^= pos.Col ^ pos.Row;
            }
            return result;
        }
    };

    size_t result = std::numeric_limits<size_t>::max();
    int tracker = 0;
    std::unordered_map<History, size_t, HistoryHash> seen;
    std::vector<State> queue;
    queue.push_back({ startingPoints, 0, 0 });
    while (!queue.empty()) {
        auto current = queue.back();
        queue.pop_back();

        if (tracker++ % 1000 == 0) {
            std::cout << "QueueSize: " << queue.size() << " Best: " << result << "\n";
        }
        if (current.Keys == allKeys) {
            result = std::min(result, current.Steps);
            continue;
        }

        for (size_t bot = 0; bot < startingPoints.size(); bot++) {
            for (auto key : reachableKeys[bot]) {
                auto keyBit = key - 'a';
                if (Constexpr::IsBitSet(current.Keys, keyBit)) continue;
                auto distance = GetPathDistance(map, current.BotPositions[bot], doorMap.at(key), current.Keys, limit);
                if (distance == 0) continue;
                State next = current;
                next.BotPositions[bot] = doorMap.at(key);
                Constexpr::SetBit(next.Keys, keyBit);
                History history = { next.BotPositions, next.Keys };
                if (seen.contains(history)) {
                    if (seen.at(history) <= next.Steps + distance) continue;
                }
                next.Steps += distance;
                seen[history] = next.Steps;
                queue.push_back(next);
            }
        }
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "#######",
        "#a.#Cd#",
        "##...##",
        "##.@.##",
        "##...##",
        "#cB#Ab#",
        "#######"
    };

    if (PartTwo(lines) != 8) return false;

    lines = {
        "#############",
        "#g#f.D#..h#l#",
        "#F###e#E###.#",
        "#dCba...BcIJ#",
        "#####.@.#####",
        "#nK.L...G...#",
        "#M###N#H###.#",
        "#o#m..#i#jk.#",
        "#############"
    };

    if (PartTwo(lines) != 72) return false;

    return true;
}