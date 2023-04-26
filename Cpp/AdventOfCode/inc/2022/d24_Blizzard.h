#pragma once

#include "Common.h"
#include "AStar.h"

#include <iostream>

enum struct Direction { Up, Down, Left, Right };

using BlizzardDirection = std::vector<std::vector<bool>>;
using Blizzard = std::unordered_map<Direction, BlizzardDirection>;

RowCol GetStart() {
    return { 0, 1 };
}

RowCol GetExit(const std::vector<std::string>& lines) {
    return { lines.size() - 1, lines[lines.size() - 1].size() - 2 };
}

RowCol GetRowColCounts(const Blizzard& blizzard) {
    auto down = blizzard.at(Direction::Down);
    auto right = blizzard.at(Direction::Right);

    return { right.size(), down[0].size()};
}

Blizzard ParseInput(const std::vector<std::string>& lines) {
    Blizzard result;
    BlizzardDirection up;
    BlizzardDirection down;
    BlizzardDirection left;
    BlizzardDirection right;

    for (const auto& line : lines) {
        std::vector<bool> upLine;
        std::vector<bool> downLine;
        std::vector<bool> leftLine;
        std::vector<bool> rightLine;
        for (auto c : line) {
            bool isLeft = false;
            bool isRight = false;
            bool isUp = false;
            bool isDown = false;
            switch (c) {
            case '<': isLeft = true; break;
            case '>': isRight = true; break;
            case 'v': isDown = true; break;
            case '^': isUp = true; break;
            default: break;
            }
            upLine.push_back(isUp);
            downLine.push_back(isDown);
            leftLine.push_back(isLeft);
            rightLine.push_back(isRight);
        }

        up.push_back(upLine);
        down.push_back(downLine);
        left.push_back(leftLine);
        right.push_back(rightLine);
    }

    result[Direction::Up] = up;
    result[Direction::Down] = down;
    result[Direction::Left] = left;
    result[Direction::Right] = right;

    return result;
}

Blizzard GetBlizzardAtTime(const Blizzard& blizzard, u32 timeStep) {
    Blizzard result = blizzard;
    auto& up = result.at(Direction::Up);
    auto& down = result.at(Direction::Down);
    auto& left = result.at(Direction::Left);
    auto& right = result.at(Direction::Right);

    auto limits = GetRowColCounts(blizzard);
    auto rows = limits.Row - 2;
    auto cols = limits.Col - 2;
    //need to skip the walls on both sides

    auto yOffset = timeStep % rows;
    auto xOffset = timeStep % cols;

    for (auto row = 1; row <= rows; row++) {
        std::rotate(left[row].begin() + 1, left[row].begin() + 1 + xOffset, left[row].end() - 1);
        std::rotate(right[row].rbegin() + 1, right[row].rbegin() + 1 + xOffset, right[row].rend() - 1);
    }

    std::rotate(up.begin() + 1, up.begin() + 1 + yOffset, up.end() - 1);
    std::rotate(down.rbegin() + 1, down.rbegin() + 1 + yOffset, down.rend() - 1);

    return result;
}

std::vector<RowCol> GetOpenPositions(const Blizzard& blizzard) {
    auto up = blizzard.at(Direction::Up);
    auto down = blizzard.at(Direction::Down);
    auto left = blizzard.at(Direction::Left);
    auto right = blizzard.at(Direction::Right);

    auto limits = GetRowColCounts(blizzard);

    std::vector<RowCol> result;

    for (size_t row = 1; row < limits.Row; row++) {
        for (size_t col = 1; col < limits.Col; col++) {
            if (!(up[row][col] || down[row][col] || left[row][col] || right[row][col])) {
                result.push_back({ row, col });
            }
        }
    }

    return result;
}

void PrintBlizzard(const Blizzard& blizzard, const RowCol& santaPos) {
    auto limits = GetRowColCounts(blizzard);
    RowCol start = { 0, 1 };
    RowCol end = { limits.Row - 1, limits.Col - 2 };

    for (auto row = 0; row < limits.Row; row++) {
        for (auto col = 0; col < limits.Col; col++) {
            if (row == 0 || col == 0) {
                std::cout << '#';
            }
            else if (row == limits.Row - 1 || col == limits.Col - 1) {
                std::cout << '#';
            }
            else {
                bool up = blizzard.at(Direction::Up)[row][col];
                bool down = blizzard.at(Direction::Down)[row][col];
                bool left = blizzard.at(Direction::Left)[row][col];
                bool right = blizzard.at(Direction::Right)[row][col];
                bool santa = santaPos.Row == row && santaPos.Col == col;

                auto count = up + down + left + right;
                if (count == 1) {
                    if (santa) throw std::logic_error("Santa's standing in a blizzard");

                    if (up) std::cout << '^';
                    if (down) std::cout << 'v';
                    if (left) std::cout << '<';
                    if (right) std::cout << '>';
                }
                else if (count == 0) {
                    if (santa) {
                        std::cout << "S";
                    }
                    else {
                        std::cout << '.';
                    }
                }
                else {
                    if (santa) throw std::logic_error("Santa's standing in at least 2 blizzards");

                    std::cout << count;
                }
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::vector<RowCol> GetNeighbors(const RowCol& pos, const RowCol& exit, const RowCol& limits) {
    std::vector<RowCol> nextPositions;
    if (MDistance(pos, exit) == 1) {
        nextPositions.push_back(exit);
    }

    if (pos.Row > 1) {
        nextPositions.push_back({ pos.Row - 1, pos.Col });
    }
    if (pos.Row < limits.Row - 2) {
        nextPositions.push_back({ pos.Row + 1, pos.Col });
    }
    if (pos.Col > 1 && pos.Row < limits.Row - 1) {
        nextPositions.push_back({ pos.Row, pos.Col - 1 });
    }
    if (pos.Col < limits.Col - 2 && pos.Row > 0 && pos.Row < limits.Row - 1) {
        nextPositions.push_back({ pos.Row, pos.Col + 1 });
    }
    nextPositions.push_back(pos);

    return nextPositions;
}

u32 Travel(const Blizzard& blizzard, RowCol start, RowCol exit, RowCol limits, u32 startTime) {
    std::unordered_map<RowCol, std::vector<u32>> openTimePoints;
    for (auto i = 0; i < 400; i++) {
        auto open = GetOpenPositions(GetBlizzardAtTime(blizzard, i + startTime));
        openTimePoints[start].push_back(i + startTime);
        openTimePoints[exit].push_back(i + startTime);
        for (auto pos : open) {
            openTimePoints[pos].push_back(i + startTime);
        }
    }

    std::vector<RowCol> current = { start };
    std::unordered_set<RowCol> next;
    std::unordered_map<RowCol, RowCol> moveFrom;

    while (true) {
        //std::cout << "Minute " << startTime << "\n";
        for (const auto& pos : current) {
            for (const auto& neighbor : GetNeighbors(pos, exit, limits)) {
                if (neighbor == exit) {
                    return startTime + 1;
                }

                if (std::find(openTimePoints.at(neighbor).begin(), openTimePoints.at(neighbor).end(), startTime + 1) != openTimePoints.at(neighbor).end()) {
                    //PrintBlizzard(GetBlizzardAtTime(blizzard, startTime + 1), neighbor);
                    if (moveFrom.find(neighbor) == moveFrom.end()) {
                        moveFrom[neighbor] = pos;
                    }
                    next.insert(neighbor);
                }
            }
        }
        startTime++;
        current.clear();
        std::copy(next.begin(), next.end(), std::back_inserter(current));
        next.clear();

        if (current.empty()) {
            break; //fail case
        }
    }

    return startTime;

}

u32 PartOne(const std::vector<std::string>& lines) {
    auto blizzard = ParseInput(lines);
    auto start = GetStart();
    auto exit = GetExit(lines);
    auto limits = GetRowColCounts(blizzard);

    return Travel(blizzard, start, exit, limits, 0);
}

u32 PartTwo(const std::vector<std::string>& lines) {
    auto blizzard = ParseInput(lines);
    auto start = GetStart();
    auto exit = GetExit(lines);
    auto limits = GetRowColCounts(blizzard);

    auto result = Travel(blizzard, start, exit, limits, 0);
    result = Travel(blizzard, exit, start, limits, result);
    result = Travel(blizzard, start, exit, limits, result);
    return result;
}

//690 is too low
std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "#.######",
        "#>>.<^<#",
        "#.<..<<#",
        "#>v.><>#",
        "#<^v^^>#",
        "######.#"
    };

    //if (PartOne(lines) != 18) return false;
    if (PartTwo(lines) != 54) return false;
    /*
    auto blizzard = ParseInput(lines);
    auto start = GetStart();
    auto exit = GetExit(lines);
    auto limits = GetRowColCounts(blizzard);
    */

    /*
    if (Travel(blizzard, start, exit, limits, 0) != 18) return false;
    if (Travel(blizzard, start, exit, limits, 41) != 54) return false;
    */
    //if (Travel(blizzard, exit, start, limits, 18) != 41) return false;
    //if (Travel(blizzard, exit, start, limits, 18) != 41) return false;

    return true;
}