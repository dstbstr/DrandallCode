#include "2023/d18_LavaLagoon.h"
#include "Algorithms/Shoelace.h"
#include "Facing.h"

SOLUTION(2023, 18) {
    using Points = std::vector<Coord>;

    constexpr void Dig(const std::string & line, Points & points, Coord & currentPos) {
        auto s = Constexpr::Split(line, " ");
        auto dir = ParseFacing(s[0][0]);
        u32 distance;
        Constexpr::ParseNumber(s[1], distance);
        Move(currentPos, dir, distance);
        points.push_back(currentPos);
    }

    constexpr void Dig2(const std::string& line, Points& points, Coord& currentPos) {
        auto s = Constexpr::Split(line, " ");
        size_t distance = Constexpr::ParseHex(s[2].substr(2, 5));
        Facing dir = Up;
        switch (s[2][7]) {
        case '0': dir = Right; break;
        case '1': dir = Down; break;
        case '2': dir = Left; break;
        case '3': dir = Up; break;
        }

        Move(currentPos, dir, distance);
        points.push_back(currentPos);
    }

    
    constexpr size_t GetPerimeter(const Points& points) {
        size_t result = 0;
        for (auto i = 0; i < points.size() - 1; i++) {
            result += MDistance(points[i], points[i + 1]);
        }
        result += MDistance(points.back(), points[0]);
        return result;
    }

    constexpr size_t FindArea(const Points& points) {
        auto interior = static_cast<size_t>(Shoelace::GetInteriorArea(points));
        auto p = GetPerimeter(points);
        return (interior + ( p / 2) + 1); //Pick's Theorem
    }
    
    constexpr size_t Solve(const std::vector<std::string>& lines, auto Func) {
        Points points;
        Coord pos{ 0, 0 };

        for (const auto& line : lines) {
            Func(line, points, pos);
        }
        return FindArea(points);
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, Dig));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, Dig2));
    }

    TESTS() {
        std::vector<std::string> example = {
            "R 6 (#70c710)",
            "D 5 (#0dc571)",
            "L 2 (#5713f0)",
            "D 2 (#d2c081)",
            "R 2 (#59c680)",
            "D 2 (#411b91)",
            "L 5 (#8ceee2)",
            "U 2 (#caa173)",
            "L 1 (#1b58a2)",
            "U 2 (#caa171)",
            "R 2 (#7807d2)",
            "U 3 (#a77fa3)",
            "L 2 (#015232)",
            "U 2 (#7a21e3)"
        };

        if (Solve(example, Dig) != 62) return false;
        if (Solve(example, Dig2) != 952'408'144'115ull) return false;

        return true;
    }
}