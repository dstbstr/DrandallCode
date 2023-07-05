#include "2017/d24_Bridge.h"

SOLUTION(2017, 24) {
    constexpr std::vector<UCoord> ParsePieces(const std::vector<std::string>&lines) {
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

    constexpr u64 CalculateStrength(const std::vector<UCoord>&pieces) {
        u64 result = 0;
        for (const auto& piece : pieces) {
            result += piece.X + piece.Y;
        }

        return result;
    }

    size_t Dfs(size_t pieceNeeded, std::vector<UCoord> pieces, size_t currentStrength) {
        size_t best = currentStrength;

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

    auto Part1(const std::vector<std::string>&lines) {
        auto pieces = ParsePieces(lines);
        return Dfs(0, pieces, 0);
    }

    std::vector<UCoord> FindLongestBridge(size_t pieceNeeded, std::vector<UCoord> pieces, std::vector<UCoord> currentBridge) {
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

    auto Part2(const std::vector<std::string>&lines) {
        auto pieces = ParsePieces(lines);
        return CalculateStrength(FindLongestBridge(0, pieces, {}));
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
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

        if (Part1(lines) != 31) return false;
        if (Part2(lines) != 19) return false;

        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}