#pragma once

#include "Common.h"

enum struct Direction {Up, Down, Left, Right};

/*
std::unordered_map<u32, std::unordered_map<Direction, u32>> lookup {
    {1, {
        {Direction::Up, 1},
        {Direction::Down, 4},
        {Direction::Left, 1},
        {Direction::Right, 2}
    }},
    {2, {
        {Direction::Up, 2},
        {Direction::Down, 5},
        {Direction::Left, 1},
        {Direction::Right, 3}
    }},
    {3, {
        {Direction::Up, 3},
        {Direction::Down, 6},
        {Direction::Left, 2},
        {Direction::Right, 3}
    }},
    {4, {
        {Direction::Up, 1},
        {Direction::Down, 7},
        {Direction::Left, 4},
        {Direction::Right, 5}
    }},
    {5, {
        {Direction::Up, 2},
        {Direction::Down, 8},
        {Direction::Left, 4},
        {Direction::Right, 6}
    }},
    {6, {
        {Direction::Up, 3},
        {Direction::Down, 9},
        {Direction::Left, 5},
        {Direction::Right, 6}
    }},
    {7, {
        {Direction::Up, 4},
        {Direction::Down, 7},
        {Direction::Left, 7},
        {Direction::Right, 8}
    }},
    {8, {
        {Direction::Up, 5},
        {Direction::Down, 8},
        {Direction::Left, 7},
        {Direction::Right, 9}
    }},
    {9, {
        {Direction::Up, 6},
        {Direction::Down, 9},
        {Direction::Left, 8},
        {Direction::Right, 9}
    }}
};

std::unordered_map<std::string, std::unordered_map<Direction, std::string>> grossLookup = {
    {"1", {
        {Direction::Up, "1"},
        {Direction::Down, "3"},
        {Direction::Left, "1"},
        {Direction::Right, "1"}
    }},
    {"2", {
        {Direction::Up, "2"},
        {Direction::Down, "6"},
        {Direction::Left, "2"},
        {Direction::Right, "3"}
    }},
    {"3", {
        {Direction::Up, "1"},
        {Direction::Down, "7"},
        {Direction::Left, "2"},
        {Direction::Right, "4"}
    }},
    {"4", {
        {Direction::Up, "4"},
        {Direction::Down, "8"},
        {Direction::Left, "3"},
        {Direction::Right, "4"}
    }}
};
*/
constexpr Direction ParseDirection(const char letter) {
    switch(letter) {
        case 'U': return Direction::Up;
        case 'D': return Direction::Down;
        case 'L': return Direction::Left;
        default: case 'R': return Direction::Right;
    }
}

using LookupType = std::unordered_map<std::string, std::unordered_map<Direction, std::string>>;

template<size_t Rows, size_t Cols>
LookupType BuildMap(std::vector<std::string> input) {
    auto result = LookupType{};

    for(size_t row = 0; row < Rows; row++) {
        for(size_t col = 0; col < Cols; col++) {
            auto current = input[row * Rows + col];
            auto upIndex = (row - 1) * Rows + col;
            auto downIndex = (row + 1) * Rows + col;
            auto rightIndex = row * Rows + col + 1;
            auto leftIndex = row * Rows + col - 1;

            if(current == " ") continue;
            if(col == 0 || input[leftIndex] == " ") {
                result[current][Direction::Left] = current;
            } else {
                result[current][Direction::Left] = input[leftIndex];
            }
            
            if(row == 0 || input[upIndex] == " ") {
                result[current][Direction::Up] = current;
            } else {
                result[current][Direction::Up] = input[upIndex];
            }

            if(col == Cols -1 || input[rightIndex] == " ") {
                result[current][Direction::Right] = current;
            } else {
                result[current][Direction::Right] = input[rightIndex];
            }

            if(row == Rows -1 || input[downIndex] == " ") {
                result[current][Direction::Down] = current;
            } else {
                result[current][Direction::Down] = input[downIndex];
            }
        }
    }

    return result;
}

std::vector<std::string> GetCode(const std::vector<std::string>& lines, LookupType& lookup) {

    std::string key = "5";
    std::vector<std::string> result;

    for(auto line : lines) {
        for(auto c : line) {
            key = lookup[key][ParseDirection(c)];
        }
        result.push_back(key);
    }

    return result;
}

std::string GetSimpleCode(const std::vector<std::string>& lines) {
    auto lookup = BuildMap<3, 3>({
        "1", "2", "3",
        "4", "5", "6",
        "7", "8", "9"
        }
    );

    auto keys = GetCode(lines, lookup);
    return StrUtil::JoinVec("", keys);
}

std::string GetRealCode(const std::vector<std::string>& lines) {
    auto lookup = BuildMap<5, 5>({
        " ", " ", "1", " ", " ",
        " ", "2", "3", "4", " ",
        "5", "6", "7", "8", "9",
        " ", "A", "B", "C", " ",
        " ", " ", "D", " ", " "
    });

    auto keys = GetCode(lines, lookup);
    return StrUtil::JoinVec("", keys);
}

bool RunTests() 
{
    std::vector<std::string> lines;
    lines.push_back("ULL");
    lines.push_back("RRDDD");
    lines.push_back("LURDL");
    lines.push_back("UUUUD");

    auto result = GetSimpleCode(lines);
    if(result != "1985") return false;

    return true;
}