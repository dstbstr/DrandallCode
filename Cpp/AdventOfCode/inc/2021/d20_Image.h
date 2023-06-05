#pragma once

#include "Common.h"

using Image = std::vector<std::vector<bool>>;

constexpr Image ParseInput(const std::vector<std::string>& lines) {
    Image result;
    for (const auto& line : lines) {
        std::vector<bool> current;
        for (auto c : line) {
            current.push_back(c == '#');
        }
        result.push_back(current);
    }
    return result;
}

constexpr bool GetNextPixel(const Image& current, const std::string& key, s32 row, s32 col, char defVal) {
    std::string bits;
    for (s32 r = row - 1; r < row + 2; r++) {
        for (s32 c = col - 1; c < col + 2; c++) {
            if (r < 0 || c < 0) bits.push_back(defVal);
            else if (r > current.size() - 1 || c > current.size() - 1) bits.push_back(defVal);
            else bits.push_back(current[r][c] ? '#' : '.');
        }
    }

    auto index = Constexpr::FromBinary<size_t, '#', '.'>(bits);
    return key[index] == '#';
}

constexpr Image Next(const Image& current, const std::string& key, bool firstTime) {
    Image result;
    auto defVal = '.';
    if (key[0] == '#') {
        defVal = (firstTime ? key.back() : key[0]);
    }
    for (s32 row = -5; row < static_cast<s32>(current.size() + 5); row++) {
        std::vector<bool> currentRow;
        for (s32 col = -5; col < static_cast<s32>(current[0].size() + 5); col++) {
            currentRow.push_back(GetNextPixel(current, key, row, col, defVal));
        }
        result.push_back(currentRow);
    }
    return result;
}

constexpr void Enhance(const std::string& key, Image& image, size_t count) {
    for (auto i = 0; i < count; i++) {
        image = Next(image, key, i % 2 == 0);
    }
}

constexpr size_t Execute(const std::vector<std::string>& lines, size_t count) {
    auto groups = SplitInputIntoGroups(lines);
    const auto key = groups[0][0];
    auto image = ParseInput(groups[1]);

    Enhance(key, image, count);

    size_t result = 0;
    for (const auto& row : image) {
        result += std::count(row.begin(), row.end(), true);
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(Execute(lines, 2));
    return ToString(Execute(lines, 50));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#",
        "",
        "#..#.",
        "#....",
        "##..#",
        "..#..",
        "..###"
    };

    if (Execute(lines, 2) != 35) return false;
    //if (Execute(lines, 50) != 3351) return false;
    return true;
}