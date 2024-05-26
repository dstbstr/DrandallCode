#include "Core/Constexpr/ConstexprMatrix.h"

namespace Constexpr {
    constexpr bool TestFlipX() {
        Mat<int, 2> i = { {{1, 2}, {3, 4}} };
        Mat<int, 2> e = { {{3, 4}, {1, 2}} };
        if (Constexpr::FlipX(i) != e) return false;

        Mat<int, 3> i2 = { {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}} };
        Mat<int, 3> e2 = { {{7, 8, 9}, {4, 5, 6}, {1, 2, 3}} };
        if (Constexpr::FlipX(i2) != e2) return false;
        return true;
    }

    constexpr bool TestFlipY() {
        Mat<int, 2> i = { { {1, 2}, {3, 4} } };
        Mat<int, 2> e = { { {2, 1}, {4, 3} } };
        if (Constexpr::FlipY(i) != e) return false;

        Mat<int, 3> i2 = { { { 1, 2, 3 }, {4, 5, 6}, {7, 8, 9} } };
        Mat<int, 3> e2 = { { {3, 2, 1}, {6, 5, 4}, {9, 8, 7} } };
        if (Constexpr::FlipY(i2) != e2) return false;

        return true;
    }

    constexpr bool TestRotate() {
        Mat<int, 3> initial = { { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} } };
        Mat<int, 3> expected = { { {7, 4, 1}, {8, 5, 2}, {9, 6, 3} } };
        if (Constexpr::Rotate(initial) != expected) return false;

        return true;
    }

    constexpr bool TestDeterminant() {
        Mat<int, 2> mat2d = { { {4, 7}, {2, 6} } };
        if (Determinant(mat2d) != 10) return false;

        Mat<int, 3> mat3d = { {{1, -1, 0}, {2, 3, 4}, {0, 1, 2}} };
        if (Determinant(mat3d) != 6) return false;

        return true;
    }

    constexpr bool TestMultiplyMatrix() {
        Mat<int, 2> lhs = { {{1, 2}, {3, 4}} };
        Mat<int, 2> rhs = { {{2, 0}, {1, 2}} };

        Mat<int, 2> expected1 = { {{4, 4}, {10, 8}} };
        if (MultiplyMatrix(lhs, rhs) != expected1) return false;

        Mat<int, 2> expected2 = { {{2, 4}, {7, 10}} };
        if (MultiplyMatrix(rhs, lhs) != expected2) return false;

        Mat<int, 2, 3> m{ {{1, -1, 2}, {0, -3, 1}} };
        Mat<int, 3, 1> v{ {{2},{1}, {0}} };

        if(MultiplyMatrix(m, v) != Mat<int, 2, 1>{{{1}, { -3 }}}) return false;

        return true;
    }

    constexpr bool TestInvertMatrix() {
        Mat<double, 2> mat2d = { {{4, 7}, {2, 6}} };
        Mat<double, 2> id2 = { { {1, 0}, {0, 1} } };
        auto result = MultiplyMatrix(mat2d, InvertMatrix(mat2d));

        for (auto i = 0; i < 2; i++) {
            for (auto j = 0; j < 2; j++) {
                if (Constexpr::AbsDistance(result[i][j], id2[i][j]) > 0.0001) return false;
            }
        }
        
        Mat<double, 3> mat3d = { {{3, 0, 2}, {2, 0, -2}, {0, 1, 1}} };
        Mat<double, 3> id3 = { {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}} };

        if (MultiplyMatrix(mat3d, InvertMatrix(mat3d)) != id3) return false;

        return true;
    }

	bool RunMatrixTests() {
        static_assert(TestFlipX());
        if (!TestFlipX()) return false;

        static_assert(TestFlipY());
        if (!TestFlipY()) return false;

        static_assert(TestRotate());
        if (!TestRotate()) return false;

        static_assert(TestMultiplyMatrix());
        if (!TestMultiplyMatrix()) return false;

        static_assert(TestDeterminant());
        if (!TestDeterminant()) return false;

        static_assert(TestInvertMatrix());
        if (!TestInvertMatrix()) return false;

        return true;
	}
}