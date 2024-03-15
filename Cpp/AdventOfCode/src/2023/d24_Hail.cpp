#include "2023/d24_Hail.h"

SOLUTION(2023, 24) {
    struct S1 {
        Vec2<double> Pos;
        Vec2<double> Vel;
    };

    struct S2 {
        Vec3<double> Pos;
        Vec3<double> Vel;
    };

    constexpr S2 ParsePartTwo(const std::string& line) {
        S2 result;
        auto s = Constexpr::Split(line, " @ ");
        result.Pos = Vec3<double>(s[0], ", ");
        result.Vel = Vec3<double>(s[1], ", ");

        return result;
    }
    
    constexpr S1 ParsePartOne(const std::string& line) {
        auto s2 = ParsePartTwo(line);

        return { {s2.Pos.X, s2.Pos.Y}, {s2.Vel.X, s2.Vel.Y} };
    }

    constexpr size_t SolvePartOne(const std::vector<std::string>& lines, Vec2<double> targetCorner, double targetSize) {
        auto stones = ParseLines(lines, ParsePartOne);

        Rect<double> target{ targetCorner, targetSize, targetSize };
        Vec2<double> intersection;
        double s, t;

        size_t result = 0;
        for (auto i = 0; i < stones.size(); i++) {
            auto a = stones[i];
            for (auto j = i + 1; j < stones.size(); j++) {
                auto b = stones[j];
                if (!Constexpr::TryFindIntersection(a.Pos, a.Vel, b.Pos, b.Vel, intersection, s, t)) continue;
                if (s < 0 || t < 0 || !target.Contains(intersection)) continue;
                result++;
            }
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines, { 200'000'000'000'000, 200'000'000'000'000 }, 200'000'000'000'000));
    }

    struct CoordTimes {
        Vec3<double> Pos;
        double T1;
        double T2;
    };

    bool TryFindIntersectionXY(const S2& a, const S2& b, CoordTimes& outResult) {
        if (b.Vel.X == 0) {
            if (a.Vel.X == 0) return false;
            if (TryFindIntersectionXY(b, a, outResult)) {
                std::swap(outResult.T1, outResult.T2);
                return true;
            }
            return false;
        }

        auto den = b.Vel.X * a.Vel.Y - b.Vel.Y * a.Vel.X;
        if (den == 0) return false;
        auto num = b.Vel.Y * (a.Pos.X - b.Pos.X) - b.Vel.X * (a.Pos.Y - b.Pos.Y);
        outResult.T1 = num / den;
        outResult.T2 = ((a.Pos.X - b.Pos.X) + a.Vel.X * outResult.T1) / b.Vel.X;

        outResult.Pos.X = a.Pos.X + a.Vel.X * outResult.T1;
        outResult.Pos.Y = a.Pos.Y + a.Vel.Y * outResult.T1;
        outResult.Pos.Z = a.Pos.Z + a.Vel.Z * outResult.T1;
        return true;
    }

    PART_TWO() {
        auto stones = ParseLines(lines, ParsePartTwo);

        auto IsInt = [](double d) {
            return Constexpr::AbsDistance(d, std::floor(d)) < 0.0000001;
            };

        auto s0 = stones[1];
        auto s1 = stones[2];
        auto s2 = stones[3];
        auto p1 = s0.Pos;
        auto p2 = s1.Pos;
        auto v1 = s0.Vel;
        auto v2 = s1.Vel;

        for (auto vx = -1000; vx < 1000; vx++) {
            for (auto vy = -1000; vy < 1000; vy++) {
                if (vy == v1.Y) continue;

                auto num = (p2.Y - p1.Y) * (vx - v1.X) - (p2.X - p1.X) * (vy - v1.Y);
                auto den = (vy - v2.Y) * (vx - v1.X) - (vx - v2.X) * (vy - v1.Y);
                if (den == 0) continue;

                auto t2 = num / den;
                if (!IsInt(t2)) continue;

                auto t1 = ((vy - v2.Y) * t2 - (p2.Y - p1.Y)) / (vy - v1.Y);
                if (t1 == t2 || !IsInt(t1)) continue;

                auto z1 = p1.Z + t1 * v1.Z;
                auto z2 = p2.Z + t2 * v2.Z;
                auto vz = (z2 - z1) / (t2 - t1);

                if (!IsInt(vz)) continue;

                auto px = p1.X + (v1.X - vx) * t1;
                auto py = p1.Y + (v1.Y - vy) * t1;
                auto pz = p1.Z + (v1.Z - vz) * t1;

                S2 s;
                s.Pos = { px, py, pz };
                s.Vel = { static_cast<double>(vx), static_cast<double>(vy), static_cast<double>(vz) };

                CoordTimes hit2;
                if (TryFindIntersectionXY(s, s2, hit2)) {
                    if (Constexpr::AbsDistance(hit2.T1, hit2.T2) < 0.001) {
                        auto t3 = hit2.T1;
                        auto z3 = s2.Pos.Z + t3 * s2.Vel.Z;
                        auto sz3 = s.Pos.Z + t3 * s.Vel.Z;
                        if (Constexpr::AbsDistance(z3, sz3) < 0.001) {
                            return Constexpr::ToString(static_cast<size_t>(px + py + pz));
                        }
                    }
                }
            }
        }
        return "Not Found";
        //probably has floating point errors or something, answer was 9 too high
        /*
        auto GetMatrixRow = [&](S2 i, S2 j) {
            auto v = CrossProduct(i.Vel - j.Vel, i.Pos - j.Pos);
            return std::array<double, 3>{v.X, v.Y, v.Z};
            };
        auto GetScalar = [](S2 i, S2 j) {
            return DotProduct(i.Vel - j.Vel, CrossProduct(i.Pos, j.Pos));
            };

        Mat<double, 3> matrix;
        Mat<double, 3, 1> vec;

        matrix[0] = GetMatrixRow(stones[0], stones[1]);
        vec[0][0] = GetScalar(stones[0], stones[1]);

        matrix[1] = GetMatrixRow(stones[0], stones[2]);
        vec[1][0] = GetScalar(stones[0], stones[2]);

        matrix[2] = GetMatrixRow(stones[1], stones[2]);
        vec[2][0] = GetScalar(stones[1], stones[2]);

        auto inverseMatrix = Constexpr::InvertMatrix(matrix);
        auto result = Constexpr::MultiplyMatrix(inverseMatrix, vec);
        
        return Constexpr::ToString(static_cast<size_t>(result[0][0] + result[1][0] + result[2][0]));
        */
    }

    TESTS() {
        std::vector<std::string> example = {
            "19, 13, 30 @ -2,  1, -2",
            "18, 19, 22 @ -1, -1, -2",
            "20, 25, 34 @ -2, -2, -4",
            "12, 31, 28 @ -1, -2, -1",
            "20, 19, 15 @  1, -5, -3"
        };

        if (SolvePartOne(example, { 7, 7 }, 20) != 2) return false;

        if (PartTwo(example) != "47") return false;

        return true;
    }
}