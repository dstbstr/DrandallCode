#pragma once

#include "Common.h"
#include <iostream>

using Map = std::array<std::array<bool, 500>, 500>;

using Action = std::function<bool(RowCol&, const Map&)>;

std::vector<std::vector<bool>> ReduceMap(const Map& map) {
    UCoord min{ 500, 500 };
    UCoord max{ 0, 0 };

    for (u32 row = 0; row < map.size(); row++) {
        for (u32 col = 0; col < map[row].size(); col++) {
            if (!map[row][col]) continue;
            min.X = std::min(min.X, col);
            min.Y = std::min(min.Y, row);
            max.X = std::max(max.X, col);
            max.Y = std::max(max.Y, row);
        }
    }

    std::vector<std::vector<bool>> result;
    for (auto row = min.Y; row <= max.Y; row++) {
        std::vector<bool> rowValues;
        for (auto col = min.X; col <= max.X; col++) {
            rowValues.push_back(map[row][col]);
        }
        result.push_back(rowValues);
    }

    return result;
}

void PrintMap(const Map& map) {
    auto reduced = ReduceMap(map);
    for (auto row : reduced) {
        for (auto col : row) {
            std::cout << (col ? '#' : '.');
        }
        std::cout << '\n';
    }
    std::cout << "\n";
}

std::vector<Action> GetActions() {
    auto isAlone = [](RowCol& pos, const Map& map) -> bool {
        for (auto row = pos.Row - 1; row < pos.Row + 2; row++) {
            for (auto col = pos.Col - 1; col < pos.Col + 2; col++) {
                if (map[row][col]) {
                    if (row == pos.Row && col == pos.Col) continue;
                    return true;
                }
            }
        }

        return false;
    };

    auto checkNorth = [](RowCol& pos, const Map& map) -> bool {
        for (auto col = pos.Col - 1; col < pos.Col + 2; col++) {
            if (map[pos.Row - 1][col]) {
                return true;
            }
        }

        pos.Row--;
        return false;
    };

    auto checkSouth = [](RowCol& pos, const Map& map) -> bool {
        for (auto col = pos.Col - 1; col < pos.Col + 2; col++) {
            if (map[pos.Row + 1][col]) {
                return true;
            }
        }

        pos.Row++;
        return false;
    };

    auto checkEast = [](RowCol& pos, const Map& map) -> bool {
        for (auto row = pos.Row - 1; row < pos.Row + 2; row++) {
            if (map[row][pos.Col + 1]) {
                return true;
            }
        }

        pos.Col++;
        return false;
    };

    auto checkWest = [](RowCol& pos, const Map& map) -> bool {
        for (auto row = pos.Row - 1; row < pos.Row + 2; row++) {
            if (map[row][pos.Col - 1]) {
                return true;
            }
        }

        pos.Col--;
        return false;
    };

    return { isAlone, checkNorth, checkSouth, checkWest, checkEast };
}

void RotateActions(std::vector<Action>& actions) {
    std::rotate(actions.begin() + 1, actions.begin() + 2, actions.end());
}

Map ParseMap(const std::vector<std::string>& lines) {
    Map result;
    for (auto& row : result) {
        row.fill(false);
    }

    for (auto row = 0; row < lines.size(); row++) {
        for (auto col = 0; col < lines[row].size(); col++) {
            result[row + 100][col + 100] = lines[row][col] == '#';
        }
    }

    return result;
}

RowCol RunActions(RowCol pos, const Map& map, const std::vector<Action> actions) {
    RowCol result = pos;
    for (auto action : actions) {
        if (!action(result, map)) {
            break;
        }
    }

    return result;
}

bool RunRound(Map& map, std::vector<Action>& actions) {
    std::vector<std::pair<RowCol, RowCol>> moves;
    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col++) {
            if (!map[row][col]) continue;

            RowCol pos = { row, col };
            auto newPos = RunActions(pos, map, actions);
            if (pos != newPos) {
                moves.push_back(std::make_pair(pos, newPos));
            }
        }
    }

    if (moves.empty()) {
        return false;
    }

    std::vector<std::pair<RowCol, RowCol>> resets;

    for (auto i = 0; i < moves.size() - 1; i++) {
        auto m1 = moves[i];
        bool match = false;

        for (auto j = i + 1; j < moves.size(); j++) {
            auto m2 = moves[j];
            if (m1.second == m2.second) {
                resets.push_back(m2);
                match = true;
            }
        }
        if (match) {
            resets.push_back(m1);
        }
    }

    std::vector<std::pair<RowCol, RowCol>> keep;
    std::copy_if(moves.begin(), moves.end(), std::back_inserter(keep), [&resets](std::pair<RowCol, RowCol> pair) {
        return std::find(resets.cbegin(), resets.cend(), pair) == resets.cend();
        });

    for (auto [src, dest] : keep) {
        map[src.Row][src.Col] = false;
        map[dest.Row][dest.Col] = true;
    }

    RotateActions(actions);
    return !keep.empty();
}

u32 PartOne(const std::vector<std::string>& lines) {
    auto actions = GetActions();
    auto map = ParseMap(lines);

    for (auto round = 0; round < 10; round++) {
        RunRound(map, actions);
    }

    auto reduced = ReduceMap(map);

    u32 result = 0;
    for (auto row : reduced) {
        for (auto elf : row) {
            if (!elf) result++;
        }
    }
    return result;
}

u32 PartTwo(const std::vector<std::string>& lines) {
    auto actions = GetActions();
    auto map = ParseMap(lines);

    u32 round = 0;
    while (RunRound(map, actions)) {
        round++;
    }

    return round + 1;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "..............",
        "..............",
        ".......#......",
        ".....###.#....",
        "...#...#.#....",
        "....#...##....",
        "...#.###......",
        "...##.#.##....",
        "....#..#......",
        "..............",
        "..............",
        ".............."
    };

    //if (PartOne(lines) != 110) return false;
    if (PartTwo(lines) != 20) return false;
    return true;
}