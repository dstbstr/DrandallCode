#pragma once

#include "Common.h"

constexpr std::vector<UCoord> ParsePieces(const std::vector<std::string>& lines) {
    std::vector<UCoord> result;
    for (const auto& line : lines) {
        auto split = Constexpr::Split(line, "/");
        UCoord coord;
        Constexpr::ParseNumber(split[0], coord.X);
        Constexpr::ParseNumber(split[1], coord.Y);
        result.push_back(coord);
    }

    return result;
}

constexpr u32 CalculateStrength(const std::vector<UCoord>& pieces) {
    u32 result = 0;
    for (const auto& piece : pieces) {
        result += piece.X + piece.Y;
    }

    return result;
}

u32 Dfs(u32 pieceNeeded, std::vector<UCoord> pieces, u32 currentStrength) {
    u32 best = currentStrength;

    for (auto i = 0; i < pieces.size(); i++) {
        auto piece = pieces[i];
        if (piece.X != pieceNeeded && piece.Y != pieceNeeded) continue;
        auto nextNeeded = piece.X == pieceNeeded ? piece.Y : piece.X;
        auto newStrength = currentStrength + piece.X + piece.Y;
        pieces.erase(pieces.begin() + i);
        best = std::max(best, Dfs(nextNeeded, pieces, newStrength));
        pieces.insert(pieces.begin() + i, piece);
    }

    return best;
}

auto PartOne(const std::vector<std::string>& lines) {
    auto pieces = ParsePieces(lines);
    return Dfs(0, pieces, 0);
}

std::vector<UCoord> FindLongestBridge(u32 pieceNeeded, std::vector<UCoord> pieces, std::vector<UCoord> currentBridge) {
    std::vector<UCoord> result = currentBridge;

    for (auto i = 0; i < pieces.size(); i++) {
        auto piece = pieces[i];
        if (piece.X != pieceNeeded && piece.Y != pieceNeeded) continue;
        auto nextNeeded = piece.X == pieceNeeded ? piece.Y : piece.X;
        pieces.erase(pieces.begin() + i);
        currentBridge.push_back(piece);
        auto longestBridge = FindLongestBridge(nextNeeded, pieces, currentBridge);
        if (longestBridge.size() >= result.size()) {
            if (CalculateStrength(result) < CalculateStrength(longestBridge)) {
                result = longestBridge;
            }
        }
        pieces.insert(pieces.begin() + i, piece);
        currentBridge.pop_back();
    }

    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto pieces = ParsePieces(lines);
    return CalculateStrength(FindLongestBridge(0, pieces, {}));
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "0/2",
        "2/2",
        "2/3",
        "3/4",
        "3/5",
        "0/1",
        "10/1",
        "9/10"
    };

    if (PartOne(lines) != 31) return false;
    if (PartTwo(lines) != 19) return false;

    return true;
}