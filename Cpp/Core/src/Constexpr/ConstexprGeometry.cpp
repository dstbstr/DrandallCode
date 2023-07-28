#include "Constexpr/ConstexprGeometry.h"

namespace Constexpr {

    namespace ConstexprGeometryTests {
        struct Point {
            int X;
            int Y;
            constexpr bool operator==(const Point& other) const {
                return X == other.X && Y == other.Y;
            }
        };

        constexpr bool TestFlipX() {
            std::array<std::array<int, 2>, 2> i = { { {1, 2}, {3, 4} } };
            std::array<std::array<int, 2>, 2> e = { { {3, 4}, {1, 2} } };
            if (Constexpr::FlipX(i) != e) return false;

            std::array<std::array<int, 3>, 3> i2 = { { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} } };
            std::array<std::array<int, 3>, 3> e2 = { {{7, 8, 9}, {4, 5, 6}, {1, 2, 3}} };
            if (Constexpr::FlipX(i2) != e2) return false;
            return true;
        }

        constexpr bool TestFlipY() {
            std::array<std::array<int, 2>, 2> i = { { {1, 2}, {3, 4} } };
            std::array<std::array<int, 2>, 2> e = { { {2, 1}, {4, 3} } };
            if (Constexpr::FlipY(i) != e) return false;

            std::array<std::array<int, 3>, 3> i2 = { { { 1, 2, 3 }, {4, 5, 6}, {7, 8, 9} } };
            std::array<std::array<int, 3>, 3> e2 = { { {3, 2, 1}, {6, 5, 4}, {9, 8, 7} } };
            if (Constexpr::FlipY(i2) != e2) return false;

            return true;
        }

        constexpr bool TestRotate() {
            std::array<std::array<int, 3>, 3> initial = { { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} } };
            std::array<std::array<int, 3>, 3> expected = { { {7, 4, 1}, {8, 5, 2}, {9, 6, 3} } };
            if (Constexpr::Rotate(initial) != expected) return false;

            return true;
        }

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
        bool RunTests() {
            static_assert(TestFlipX());
            static_assert(TestFlipY());
            static_assert(TestRotate());
            if (!TestFlipX()) return false;
            if (!TestFlipY()) return false;
            if (!TestRotate()) return false;

            static_assert(TestDoIntersect());
            if (!TestDoIntersect()) return false;

            return true;
        }
    }
}