#include "2021/d2_Dive.h"

SOLUTION(2021, 2) {
    constexpr void Apply(Coord & pos, const std::string & line) {
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

    constexpr auto Part1(const std::vector<std::string>&lines) {
        Coord pos = { 0, 0 };
        for (const auto& line : lines) {
            Apply(pos, line);
        }

        return pos.X * pos.Y;
    }

    constexpr void Apply(Vec3<s64>&pos, const std::string & line) {
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

    constexpr auto Part2(const std::vector<std::string>&lines) {
        Vec3<s64> pos = { 0, 0, 0 };
        for (const auto& line : lines) {
            Apply(pos, line);
        }

        return pos.X * pos.Y;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "forward 5",
            "down 5",
            "forward 8",
            "up 3",
            "down 8",
            "forward 2"
        };

        if (Part1(lines) != 150) return false;
        if (Part2(lines) != 900) return false;
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