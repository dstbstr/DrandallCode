#include "2021/d5_Vents.h"

SOLUTION(2021, 5) {
    constexpr void ParseLine(std::string_view line, Coord& outStart, Coord& outEnd) {
        auto s = Constexpr::Split(line, " -> ");
        outStart = Coord(s[0]);
        outEnd = Coord(s[1]);
    }

    constexpr bool IsDiagonal(Coord start, Coord end) {
        return start.X != end.X && start.Y != end.Y;
    }

    constexpr auto Solve(const auto& lines, bool ignoreDiagonal) {
        auto* seenPtr = new std::array<std::array<u8, 1024>, 1024>();
        auto& seen = *seenPtr;
        for (const auto& line : lines) {
            Coord start, end;
            ParseLine(line, start, end);
            if (!ignoreDiagonal || !IsDiagonal(start, end)) {
                auto slope = Constexpr::GetSlope(start, end);

                for (; start != end; start += slope) {
                    seen[start.Y][start.X]++;
                }
                seen[start.Y][start.X]++;
            }
        }

        auto result = Count2D(seen, [](u8 count) { return count > 1; });
        delete seenPtr;
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Lines, true));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Lines, false));
    }

    TESTS() {
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

        if (Solve(lines, true) != 5) return false;
        if (Solve(lines, false) != 12) return false;
        return true;
    }
}