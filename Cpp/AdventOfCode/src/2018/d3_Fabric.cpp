#include "2018/d3_Fabric.h"
SOLUTION(2018, 3) {

    struct Rect {
        u32 Id;
        UCoord TopLeft;
        UCoord BottomRight;
    };

    //#123 @ 3,2: 5x4
    constexpr Rect ParseLine(const std::string & line) {
        auto split = Constexpr::Split(line, " ");
        Rect result;
        Constexpr::ParseNumber(split[0].substr(1), result.Id);
        auto posSplit = Constexpr::Split(split[2], ",");
        Constexpr::ParseNumber(posSplit[0], result.TopLeft.X);
        Constexpr::ParseNumber(posSplit[1].substr(0, posSplit[1].size() - 1), result.TopLeft.Y);

        auto hwSplit = Constexpr::Split(split[3], "x");
        u32 height;
        u32 width;
        Constexpr::ParseNumber(hwSplit[0], width);
        Constexpr::ParseNumber(hwSplit[1], height);

        result.BottomRight = { result.TopLeft.X + width - 1, result.TopLeft.Y + height - 1 };
        return result;
    }

    constexpr bool Intersects(const Rect & lhs, const Rect & rhs) {
        if (lhs.BottomRight.Y < rhs.TopLeft.Y || rhs.BottomRight.Y < lhs.TopLeft.Y) {
            return false;
        }
        else if (lhs.BottomRight.X < rhs.TopLeft.X || lhs.TopLeft.X > rhs.BottomRight.X) {
            return false;
        }
        else {
            return true;
        }
    }

    static_assert(Intersects(ParseLine("#1 @ 1,3: 4x4"), ParseLine("#2 @ 3,1: 4x4")));

    auto Part1(const std::vector<std::string>&lines) {
        std::vector<Rect> rects;
        for (const auto& line : lines) {
            rects.push_back(ParseLine(line));
        }
        auto fabric = std::make_unique<std::array<std::array<u32, 1001>, 1001>>();

        for (const auto& rect : rects) {
            for (auto x = rect.TopLeft.X; x <= rect.BottomRight.X; x++) {
                for (auto y = rect.TopLeft.Y; y <= rect.BottomRight.Y; y++) {
                    (*fabric)[x][y]++;
                }
            }
        }

        u32 count = 0;
        for (auto x = 0; x < fabric->size(); x++) {
            for (auto y = 0; y < (*fabric)[0].size(); y++) {
                if ((*fabric)[x][y] > 1) {
                    count++;
                }
            }
        }

        return count;
    }

    auto Part2(const std::vector<std::string>&lines) {
        std::vector<Rect> rects;
        for (const auto& line : lines) {
            rects.push_back(ParseLine(line));
        }

        for (auto i = 0; i < rects.size(); i++) {
            bool found = false;
            for (auto j = 0; j < rects.size(); j++) {
                if (i == j) continue;
                if (Intersects(rects[i], rects[j])) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return rects[i].Id;
            }
        }

        return 0u;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "#1 @ 1,3: 4x4",
            "#2 @ 3,1: 4x4",
            "#3 @ 5,5: 2x2"
        };

        auto r1 = ParseLine(lines[0]);
        auto r2 = ParseLine(lines[1]);
        if (!Intersects(r1, r2)) return false;
        //if (Part1(lines) != 0) return false;
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