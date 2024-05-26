#include "Core/Constexpr/ConstexprGeometry.h"

constexpr Vec2<int> Origin2 = { 0, 0 };
constexpr Vec2<int> Mid2 = { 1, 1 };
constexpr Vec2<int> End2 = { 2, 2 };

constexpr Vec3<int> Origin3 = { 0 ,0, 0 };
constexpr Vec3<int> Mid3 = { 1, 1, 1 };
constexpr Vec3<int> End3 = { 2, 2, 2 };

static_assert(std::is_same_v<Vec3<int>, std::remove_cvref_t<const Vec3<int>&>>);

static_assert(Origin2.Between(Origin2, End2));
static_assert(End2.Between(Origin2, End2));
static_assert(Mid2.Between(Origin2, End2));
static_assert(!End2.Between(Origin2, Mid2));

static_assert(Origin3.Between(Origin3, End3));
static_assert(End3.Between(Origin3, End3));
static_assert(Mid3.Between(Origin3, End3));
static_assert(!End3.Between(Origin3, Mid3));

static_assert(GetDirectNeighbors(Vec2<int>{1, 2}).size() == 4);
static_assert(GetAllNeighbors(Vec2<int>{1, 2}).size() == 8);
static_assert(GetDirectNeighbors(Vec3<int>{1, 2, 3}).size() == 6);
static_assert(GetAllNeighbors(Vec3<int>{1, 2, 3}).size() == 26);

static_assert(GetDirectNeighbors(Vec2<int>{0, 0}, Vec2<int>{100, 100}).size() == 2);
static_assert(GetDirectNeighbors(Vec3<int>{0, 0, 0}, Vec3<int>{100, 100, 100}).size() == 3);

static_assert(GetAllNeighbors(Origin2, End2).size() == 3);
static_assert(GetAllNeighbors(Origin3, End3).size() == 7);

namespace Constexpr {

    namespace ConstexprGeometryTests {
        struct Point {
            int X;
            int Y;
            constexpr bool operator==(const Point& other) const {
                return X == other.X && Y == other.Y;
            }
        };

        constexpr bool TestDoIntersect() {
            Point a, b, c, d;
            a = { 1, 1 };
            b = { 10, 1 };
            c = { 1, 2 };
            d = { 10, 2 };
            if (DoIntersect(a, b, c, d)) return false;

            a = { 10, 0 };
            b = { 0, 10 };
            c = { 0, 0 };
            d = { 10, 10 };
            if (!DoIntersect(a, b, c, d)) return false;

            a = { -5, -5 };
            b = { 0, 0 };
            c = { 1, 1 };
            d = { 10, 10 };
            if (DoIntersect(a, b, c, d)) return false;

            a = { 0, 0 };
            b = { 2, 6 };
            c = { 1, 3 };
            if (!DoIntersect(a, b, c)) return false;

            a = { 0, 0 };
            b = { 3, 9 };
            c = { 1, 3 };
            if (!DoIntersect(a, b, c)) return false;

            a = { 0, 0 };
            b = { 4, 9 };
            c = { 1, 3 };
            if (DoIntersect(a, b, c)) return false;

            return true;
        }

        constexpr bool TestTryFindIntersection() {
            Vec2<double> result;
            if (!TryFindIntersection({ 19, 13 }, { 17, 14 }, { 18, 19 }, { 17, 18 }, result)) return false;
            if (Constexpr::Abs(result.X - 14.333) > 0.01) return false;
            if (Constexpr::Abs(result.Y - 15.333) > 0.01) return false;

            if (!TryFindIntersection({ 19, 13 }, { 17, 14 }, { 20, 25 }, { 18, 23 }, result)) return false;
            if (Constexpr::Abs(result.X - 11.667) > 0.001) return false;
            if (Constexpr::Abs(result.Y - 16.667) > 0.001) return false;

            double s, t;

            if (!TryFindIntersection({ 19, 13 }, { -2, 1 }, { 18, 19 }, { -1, -1 }, result, s, t)) return false;
            if (Constexpr::Abs(result.X - 14.333) > 0.01) return false;
            if (Constexpr::Abs(result.Y - 15.333) > 0.01) return false;
            if (s <= 0 || t <= 0) return false;

            if (!TryFindIntersection({ 19, 13 }, { -2, 1 }, { 20, 19 }, { 1, -5 }, result, s, t)) return false;
            if (s >= 0) return false;
            if (t <= 0) return false;

            return true;
        }


        constexpr bool TestDotProduct() {
            if (DotProduct(Vec3<int>{ 3, -5, 4 }, Vec3<int>{ 2, 6, 5 }) != -4) return false;
            return true;
        }

        constexpr bool TestCrossProduct() {
            Vec3<int> lhs = { 3, -5, 4 };
            Vec3<int> rhs = { 2, 6, 5 };

            if (CrossProduct(lhs, rhs) != Vec3<int>{-49, -7, 28}) return false;

            return true;
        }

        bool RunTests() {
            static_assert(TestDoIntersect());
            if (!TestDoIntersect()) return false;

            static_assert(TestTryFindIntersection());
            if (!TestTryFindIntersection()) return false;

            static_assert(TestDotProduct());
            if (!TestDotProduct()) return false;

            static_assert(TestCrossProduct());
            if (!TestCrossProduct()) return false;

            return true;
        }
    }
}