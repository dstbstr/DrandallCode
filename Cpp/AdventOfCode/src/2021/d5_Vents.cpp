#include "2021/d5_Vents.h"

SOLUTION(2021, 5) {
    constexpr void ParseLine(const std::string & line, UCoord & outStart, UCoord & outEnd) {
        auto s1 = Constexpr::Split(line, " -> ");
        auto start = Constexpr::Split(s1[0], ",");
        auto end = Constexpr::Split(s1[1], ",");
        outStart = { 0, 0 };
        outEnd = { 0, 0 };
        Constexpr::ParseNumber(start[0], outStart.X);
        Constexpr::ParseNumber(start[1], outStart.Y);
        Constexpr::ParseNumber(end[0], outEnd.X);
        Constexpr::ParseNumber(end[1], outEnd.Y);
    }

    constexpr bool IsDiagonal(UCoord start, UCoord end) {
        return start.X != end.X && start.Y != end.Y;
    }

    constexpr std::vector<UCoord> GetLineCoords(UCoord start, UCoord end) {
        std::vector<UCoord> result;
        if (start.X == end.X) {
            auto startY = start.Y;
            auto endY = end.Y;
            if (startY > endY) std::swap(startY, endY);

            for (auto y = startY; y <= endY; y++) {
                result.push_back({ start.X, y });
            }
        }
        else {
            auto startX = start.X;
            auto endX = end.X;
            if (startX > endX) std::swap(startX, endX);

            for (auto x = startX; x <= endX; x++) {
                result.push_back({ x, start.Y });
            }
        }
        return result;
    }

    constexpr std::vector<UCoord> GetAllLineCoords(UCoord start, UCoord end) {
        if (!IsDiagonal(start, end)) {
            return GetLineCoords(start, end);
        }

        std::vector<UCoord> result{ end };
        auto dx = start.X < end.X ? 1 : -1;
        auto dy = start.Y < end.Y ? 1 : -1;
        auto distance = static_cast<int>(Constexpr::AbsDistance(start.X, end.X));
        for (int i = 0; i < distance; i++) {
            result.push_back({ start.X + (i * dx), start.Y + (i * dy) });
        }
        return result;
    }

    auto Solve(const std::vector<std::string>&lines, bool ignoreDiagonal) {
        std::unordered_map<UCoord, u32> seen;
        for (const auto& line : lines) {
            UCoord start, end;
            ParseLine(line, start, end);
            if (!ignoreDiagonal || !IsDiagonal(start, end)) {
                auto coords = GetAllLineCoords(start, end);
                for (auto coord : coords) {
                    seen[coord]++;
                }
            }
        }

        return std::count_if(seen.begin(), seen.end(), [](const auto& kvp) {
            return kvp.second > 1;
            });
    }

    auto Part1(const std::vector<std::string>&lines) {
        return Solve(lines, true);
    }

    auto Part2(const std::vector<std::string>&lines) {
        return Solve(lines, false);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "0,9 -> 5,9",
            "8,0 -> 0,8",
            "9,4 -> 3,4",
            "2,2 -> 2,1",
            "7,0 -> 7,4",
            "6,4 -> 2,0",
            "0,9 -> 2,9",
            "3,4 -> 1,4",
            "0,0 -> 8,8",
            "5,5 -> 8,2"
        };

        if (Part1(lines) != 5) return false;
        if (Part2(lines) != 12) return false;
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