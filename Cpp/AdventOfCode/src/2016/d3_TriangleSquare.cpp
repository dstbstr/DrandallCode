#include "2016/d3_TriangleSquare.h"

SOLUTION(2016, 3) {
    constexpr bool IsValid(u32 a, u32 b, u32 c) {
        if (a > b && a > c) {
            return b + c > a;
        }
        else if (b > a && b > c) {
            return a + c > b;
        }
        else {
            return a + b > c;
        }
    }

    PART_ONE() {
        u32 result = 0;
        u32 a = 0;
        u32 b = 0;
        u32 c = 0;

        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " ");
            Constexpr::ParseNumber(split[0], a);
            Constexpr::ParseNumber(split[1], b);
            Constexpr::ParseNumber(split[2], c);

            result += IsValid(a, b, c);
        }
        return Constexpr::ToString(result);
    }

    PART_TWO() {
        u32 result = 0;
        std::array<std::array<u32, 3>, 3> tris;
        for (auto i = 0; i < lines.size(); i += 3) {
            auto split1 = Constexpr::Split(lines[i], " ");
            auto split2 = Constexpr::Split(lines[i + 1], " ");
            auto split3 = Constexpr::Split(lines[i + 2], " ");

            Constexpr::ParseNumber(split1[0], tris[0][0]);
            Constexpr::ParseNumber(split2[0], tris[0][1]);
            Constexpr::ParseNumber(split3[0], tris[0][2]);

            Constexpr::ParseNumber(split1[1], tris[1][0]);
            Constexpr::ParseNumber(split2[1], tris[1][1]);
            Constexpr::ParseNumber(split3[1], tris[1][2]);

            Constexpr::ParseNumber(split1[2], tris[2][0]);
            Constexpr::ParseNumber(split2[2], tris[2][1]);
            Constexpr::ParseNumber(split3[2], tris[2][2]);

            if (IsValid(tris[0][0], tris[0][1], tris[0][2])) {
                result++;
            }
            if (IsValid(tris[1][0], tris[1][1], tris[1][2])) {
                result++;
            }
            if (IsValid(tris[2][0], tris[2][1], tris[2][2])) {
                result++;
            }
        }

        return Constexpr::ToString(result);
    }

    TESTS() {
        static_assert(IsValid(5, 10, 25) == false);

        std::vector<std::string> lines = {
            "101 301 501",
            "102 302 502",
            "103 303 503",
            "201 401 601",
            "202 402 602",
            "203 403 603"
        };

        if (PartTwo(lines) != "6") return false;
        return true;
    }
}
