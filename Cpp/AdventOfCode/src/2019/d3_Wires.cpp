#include "2019/d3_Wires.h"
#include "Facing.h"

SOLUTION(2019, 3) {
    struct Segment {
        Vec3<s64> Start;
        Vec3<s64> End;

        constexpr bool operator==(const Segment& other) const {
            return Start == other.Start && End == other.End;
        }
    };

    constexpr std::vector<Segment> GetSegments(std::string_view line) {
        std::vector<Segment> result;
        auto s = Constexpr::Split(line, ",");
        Vec3<s64> pos;
        char dir;
        s64 amount;
        Segment segment{ pos, pos };
        for (auto step : s) {
            dir = step[0];
            Constexpr::ParseNumber(step.substr(1), amount);

            segment.Start = segment.End;
            switch (dir) {
            case 'U': segment.End.Y += amount; break;
            case 'D': segment.End.Y -= amount; break;
            case 'L': segment.End.X -= amount; break;
            case 'R': segment.End.X += amount; break;
            }

            segment.End.Z += amount;
            result.push_back(segment);
        }
        return result;
    }

    constexpr Coord FindIntersection(const Segment& s1, const Segment& s2) {
        auto h = s1.Start.X == s1.End.X ? s2 : s1;
        auto v = h == s1 ? s2 : s1;
        return { v.Start.X, h.Start.Y };
    }

    constexpr Vec3<s64> FindIntersection2(const Segment& s1, const Segment& s2) {
        auto h = s1.Start.X == s1.End.X ? s2 : s1;
        auto v = h == s1 ? s2 : s1;
        Coord point = { v.Start.X, h.Start.Y };
        auto lhs = h.End.Z - (h.End.X - point.X);
        auto rhs = v.End.Z - (v.End.Y - point.Y);

        return { v.Start.X, h.Start.Y, lhs + rhs };
    }

    constexpr void Solve(const auto& lines, auto OnIntersect) {
        auto w1 = GetSegments(lines[0]);
        auto w2 = GetSegments(lines[1]);

        for (const auto& s1 : w1) {
            for (const auto& s2 : w2) {
                if (Constexpr::DoIntersect(s1.Start, s1.End, s2.Start, s2.End)) {
                    OnIntersect(s1, s2);
                }
            }
        }

    }
    PART_ONE() {
        size_t best = 9999;
        Solve(lines, [&](const Segment& s1, const Segment& s2) {
            auto intersection = FindIntersection(s1, s2);
            auto dist = MDistance(intersection);
            if (dist > 0) best = std::min(best, dist);
            });

        return Constexpr::ToString(best);
    }


    PART_TWO() {
        s64 best = 999999;
        Solve(lines, [&](const Segment& s1, const Segment& s2) {
            auto intersection = FindIntersection2(s1, s2);
            if (intersection.X != 0 || intersection.Y != 0) {
                best = std::min(best, intersection.Z);
            }
            });
        return Constexpr::ToString(best);
    }

    TESTS() {
        return true;
    }
}