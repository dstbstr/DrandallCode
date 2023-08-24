#include "2018/d3_Fabric.h"
SOLUTION(2018, 3) {

    struct Rect {
        u32 Id{ 0 };
        UCoord TopLeft{ 0, 0 };
        UCoord BottomRight{ 0, 0 };
    };

    //#123 @ 3,2: 5x4
    constexpr Rect ParseRect(std::string_view line) {
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
        return lhs.BottomRight.Y >= rhs.TopLeft.Y &&
            rhs.BottomRight.Y >= lhs.TopLeft.Y &&
            lhs.BottomRight.X >= rhs.TopLeft.X &&
            lhs.TopLeft.X <= rhs.BottomRight.X;
    }

    PART_ONE() {
        auto rects = ParseLines(Lines, ParseRect);

        auto fabric = new std::array<std::array<u32, 1001>, 1001>();

        for (const auto& rect : rects) {
            for (auto x = rect.TopLeft.X; x <= rect.BottomRight.X; x++) {
                for (auto y = rect.TopLeft.Y; y <= rect.BottomRight.Y; y++) {
                    (*fabric)[x][y]++;
                }
            }
        }

        auto count = std::accumulate(fabric->begin(), fabric->end(), 0ull, [](size_t running, const auto& row) { 
            return running + std::count_if(row.begin(), row.end(), [](u32 v) { return v > 1; }); 
        });
        delete fabric;
        return Constexpr::ToString(count);

    }

    PART_TWO() {
        auto rects = ParseLines(Lines, ParseRect);

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
                return Constexpr::ToString(rects[i].Id);
            }
        }
        return "Not Found";
    }

    TESTS() {
        static_assert(Intersects(ParseRect("#1 @ 1,3: 4x4"), ParseRect("#2 @ 3,1: 4x4")));
        std::vector<std::string> lines = {
            "#1 @ 1,3: 4x4",
            "#2 @ 3,1: 4x4",
            "#3 @ 5,5: 2x2"
        };

        auto rects = ParseLines(lines, ParseRect);
        if (!Intersects(rects[0], rects[1])) return false;
        return true;
    }
}