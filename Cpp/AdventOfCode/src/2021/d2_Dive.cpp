#include "2021/d2_Dive.h"

SOLUTION(2021, 2) {
    constexpr void Apply(Coord & pos, std::string_view line) {
        auto split = Constexpr::Split(line, " ");
        s32 num;
        Constexpr::ParseNumber(split[1], num);

        if (split[0] == "forward") {
            pos.X += num;
        }
        else if (split[0] == "down") {
            pos.Y += num;
        }
        else if (split[0] == "up") {
            pos.Y -= num;
        }
    }

    constexpr void Apply(Vec3<s64>&pos, std::string_view line) {
        auto split = Constexpr::Split(line, " ");
        s32 num;
        Constexpr::ParseNumber(split[1], num);

        if (split[0] == "forward") {
            pos.X += num;
            pos.Y += num * pos.Z;
        }
        else if (split[0] == "down") {
            pos.Z += num;
        }
        else if (split[0] == "up") {
            pos.Z -= num;
        }
    }

    constexpr s64 Solve(const auto& lines, auto pos) {
        for (const auto& line : lines) {
            Apply(pos, line);
        }
        return pos.X * pos.Y;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, Coord{ 0, 0 }));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, Vec3<s64>{0, 0, 0}));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "forward 5",
            "down 5",
            "forward 8",
            "up 3",
            "down 8",
            "forward 2"
        };

        if (Solve(lines, Coord{ 0, 0 }) != 150) return false;
        if (Solve(lines, Vec3<s64>{0, 0, 0}) != 900) return false;
        return true;
    }
}