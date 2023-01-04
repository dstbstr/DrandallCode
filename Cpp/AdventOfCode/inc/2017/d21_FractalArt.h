#pragma once

#include "Common.h"
#include <iostream>

using Pattern = std::vector<std::vector<bool>>;

constexpr std::string ToString(const Pattern& pattern) {
    std::string result;
    for (const auto& row : pattern) {
        for (const auto& col : row) {
            result += (col ? '#' : '.');
        }
        result += '\n';
    }
    return result;
}

constexpr Pattern ParsePattern(std::string_view line) {
    Pattern result;
    auto split = Constexpr::Split(line, "/");
    for (auto row : split) {
        std::vector<bool> r;
        for (auto c : row) {
            r.push_back(c == '#');
        }
        result.push_back(r);
    }

    return result;
}

constexpr std::vector<Pattern> GetTransforms(const Pattern& input) {
    std::vector<Pattern> result;
    result.push_back(input);
    result.push_back(Constexpr::FlipX(input));
    result.push_back(Constexpr::FlipY(input));
    result.push_back(Constexpr::FlipY(result[1]));
    result.push_back(Constexpr::Rotate(input));
    result.push_back(Constexpr::Rotate(result[4]));
    result.push_back(Constexpr::Rotate(result[5]));
    result.push_back(Constexpr::FlipX(result[4]));
    result.push_back(Constexpr::FlipY(result[4]));
    result.push_back(Constexpr::FlipX(Constexpr::FlipY(result[4])));

    return result;
}

std::unordered_map<std::string, std::vector<Pattern>> ParsePatternInputs(const std::vector<std::string>& lines) {
    std::unordered_map<std::string, std::vector<Pattern>> result;

    for (const auto& line : lines) {
        auto split = StrUtil::Split(line, " => ");
        std::string key = std::string(split[0]);
        auto pattern = ParsePattern(split[0]);
        result[key] = GetTransforms(pattern);
    }

    return result;
}

std::unordered_map<std::string, Pattern> ParsePatternOutputs(const std::vector<std::string>& lines) {
    std::unordered_map<std::string, Pattern> result;

    for (const auto& line : lines) {
        auto split = StrUtil::Split(line, " => ");
        auto key = std::string(split[0]);
        auto input = ParsePattern(split[0]);
        auto output = ParsePattern(split[1]);
        result[key] = output;
    }

    return result;
}

constexpr std::vector<std::vector<Pattern>> GetChunks(Pattern pattern) {
    std::vector<std::vector<Pattern>> result;
    if (pattern.size() % 2 == 0) {
        for (auto row = 0; row < pattern.size(); row += 2) {
            std::vector<Pattern> chunkRow;
            for (auto col = 0; col < pattern.size(); col += 2) {
                Pattern chunk;
                chunk.push_back({ pattern[row].begin() + col, pattern[row].begin() + col + 2 });
                chunk.push_back({ pattern[row + 1].begin() + col, pattern[row + 1].begin() + col + 2 });
                chunkRow.push_back(chunk);
            }
            result.push_back(chunkRow);
        }
    }
    else {
        for (auto row = 0; row < pattern.size(); row += 3) {
            std::vector<Pattern> chunkRow;
            for( auto col = 0; col < pattern.size(); col += 3) {
                Pattern chunk;
                chunk.push_back({ pattern[row].begin() + col, pattern[row].begin() + col + 3 });
                chunk.push_back({ pattern[row + 1].begin() + col, pattern[row + 1].begin() + col + 3 });
                chunk.push_back({ pattern[row + 2].begin() + col, pattern[row + 2].begin() + col + 3 });
                chunkRow.push_back(chunk);
            }
            result.push_back(chunkRow);
        }
    }
    return result;
}

void UpdateChunk(Pattern& chunk, const std::unordered_map<std::string, std::vector<Pattern>>& inputPatterns, const std::unordered_map<std::string, Pattern>& outputPatterns) {
    std::string matchedPattern;
    for (const auto& [key, transforms] : inputPatterns) {
        if (transforms[0].size() != chunk[0].size()) continue;
        for (const auto& transform : transforms) {
            if (chunk == transform) {
                chunk = outputPatterns.at(key);
                return;
            }
        }
    }
}

constexpr Pattern CombineChunks(const std::vector<std::vector<Pattern>>& chunks) {
    Pattern result;
    for (const auto& chunkRow : chunks) {
        for (size_t currentRow = 0; currentRow < chunkRow[0].size(); currentRow++) {
            std::vector<bool> patternRow;
            for (const auto& chunk : chunkRow) {
                for (size_t col = 0; col < chunk.size(); col++) {
                    patternRow.push_back(chunk[currentRow][col]);
                }
            }
            result.push_back(patternRow);
        }
    }
    return result;
}

static constexpr auto Start = ".#./..#/###";

auto PartOne(const std::vector<std::string>& lines, size_t iterations) {
    auto inputPatterns = ParsePatternInputs(lines);
    auto outputPatterns = ParsePatternOutputs(lines);

    auto pattern = ParsePattern(Start);

    for (auto i = 0; i < iterations; i++) {
        //std::cout << ToString(pattern) << '\n';

        auto chunks = GetChunks(pattern);
        for (auto& chunkRow : chunks) {
            for (auto& chunk : chunkRow) {
                UpdateChunk(chunk, inputPatterns, outputPatterns);
            }
        }

        pattern = CombineChunks(chunks);
    }

    u32 count = 0;
    Constexpr::ApplyToRowCol(pattern, [&](size_t row, size_t col) {
        count += pattern[row][col];
        });
    return count;
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines, 5));
    return ToString(PartOne(lines, 18));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "../.# => ##./#../...",
        ".#./..#/### => #..#/..../..../#..#"
    };

    if (PartOne(lines, 2) != 12) return false;
    return true;
}