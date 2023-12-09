#include "2017/d24_Bridge.h"

SOLUTION(2017, 24) {
    constexpr UCoord ParsePiece(std::string_view line) {
        auto s = Constexpr::Split(line, "/");
        UCoord result;
        Constexpr::ParseNumber(s[0], result.X);
        Constexpr::ParseNumber(s[1], result.Y);
        
        return result;
    }

    constexpr size_t CalculateStrength(const std::vector<UCoord>&pieces) {
        return std::accumulate(pieces.begin(), pieces.end(), 0ull, [](size_t running, UCoord piece) {return running + piece.X + piece.Y; });
    }

    constexpr size_t Dfs(size_t pieceNeeded, std::vector<UCoord> pieces, size_t currentStrength) {
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

    constexpr std::vector<UCoord> FindLongestBridge(size_t pieceNeeded, std::vector<UCoord> pieces, std::vector<UCoord> currentBridge) {
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

    PART_ONE() {
        auto pieces = ParseLines(lines, ParsePiece);
        return Constexpr::ToString(Dfs(0, pieces, 0));
    }

    PART_TWO() {
        auto pieces = ParseLines(lines, ParsePiece);
        return Constexpr::ToString(CalculateStrength(FindLongestBridge(0, pieces, {})));
    }

    TESTS() {
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

        auto pieces = ParseLines(lines, ParsePiece);
        if (Dfs(0, pieces, 0) != 31) return false;
        if (CalculateStrength(FindLongestBridge(0, pieces, {})) != 19)  return false;

        return true;
    }
}