#pragma once
#include "Constexpr/ConstexprGeometry.h"

template<typename T, size_t Rows, size_t Cols = Rows>
using Mat = std::array<std::array<T, Cols>, Rows>;

template<typename T>
constexpr T Determinant(const Mat<T, 2>& mat) {
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

template<typename T>
constexpr T Determinant(const Mat<T, 3>& mat) {
    return (mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])) -
        (mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])) -
        (mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]));
}

namespace Constexpr {
    [[nodiscard]] constexpr auto FlipX(const auto& matrix) {
        auto result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - row][col];
            });
        return result;
    }

    [[nodiscard]] constexpr auto FlipY(const auto& matrix) {
        auto result = matrix;
        size_t n = matrix[0].size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[row][n - col];
            });

        return result;
    }

    [[nodiscard]] constexpr auto Rotate(const auto& matrix) {
        auto result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToMatrixIndex(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - col][row];
            });

        return result;
    }

    /*
    template<typename T>
    [[nodiscard]] constexpr Vec2<T> MultiplyMatrix(const Mat<T, 2>& mat, const Vec2<T>& vec) {
        Vec2<T> result;
        result.X = mat[0][0] * vec.X + mat[0][1] * vec.Y;
        result.Y = mat[1][0] * vec.X + mat[1][1] * vec.Y;

        return result;
    }
    */

    template<typename T, size_t LhsRows, size_t LhsCols, size_t RhsRows, size_t RhsCols>
    [[nodiscard]] constexpr Mat<T, LhsRows, RhsCols> MultiplyMatrix(const Mat<T, LhsRows, LhsCols>& lhs, const Mat<T, RhsRows, RhsCols>& rhs) {
        static_assert(LhsCols == RhsRows, "Lhs Rows must match Rhs Cols");

        Mat<T, LhsRows, RhsCols> result{};

        for (auto i = 0; i < LhsRows; i++) {
            for (auto j = 0; j < RhsCols; j++) {
                for (auto k = 0; k < RhsRows; k++) {
                    result[i][j] += lhs[i][k] * rhs[k][j];
                }
            }
        }

        return result;
    }
    /*
    template<typename T>
    [[nodiscard]] constexpr Vec3<T> MultiplyMatrix(const Mat<T, 3>& mat, const Vec3<T>& vec) {
        Vec3<T> result;
        result.X = mat[0][0] * vec.X + mat[0][1] * vec.Y + mat[0][2] * vec.Z;
        result.Y = mat[1][0] * vec.X + mat[1][1] * vec.Y + mat[1][2] * vec.Z;
        result.Z = mat[2][0] * vec.X + mat[2][1] * vec.Y + mat[2][2] * vec.Z;

        return result;
    }
    */

    template<typename T, size_t Rows, size_t Cols>
    [[nodiscard]] constexpr Mat<T, Rows, Cols> MultiplyMatrix(const Mat<T, Rows, Cols>& mat, T scalar) {
        auto result = mat;
        for (auto i = 0; i < Rows; i++) {
            for (auto j = 0; j < Cols; j++) {
                result[i][j] *= scalar;
            }
        }
        return result;
    }
    
    /*
    template<typename T>
    [[nodiscard]] constexpr Mat<T, 2> TransposeMatrix2D(const Mat<T, 2>& mat) {
        return { {{mat[1][1], -mat[0][1]}, {-mat[1][0], mat[0][0]}} };
    }
    */

    template<typename T, size_t Size>
    [[nodiscard]] constexpr Mat<T, Size - 1> CreateMinor(const Mat<T, Size>& mat, size_t row, size_t col) {
        Mat<T, Size - 1> r{};
        size_t ri = 0, rj = 0;
        for (auto i = 0; i < Size; i++) {
            if (i == row) continue;

            for (auto j = 0; j < Size; j++) {
                if (j == col) continue;
                r[ri][rj] = mat[i][j];
                if (rj == Size - 2) {
                    ri++;
                    rj = 0;
                }
                else {
                    rj++;
                }
            }
        }

        return r;
    }

    template<typename T, size_t Size>
    [[nodiscard]] constexpr T Determinant(const Mat<T, Size>& mat) {
        if constexpr (Size == 1) {
            return mat[0][0];
        }
        else if constexpr (Size == 2) {
            return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
        }
        else {
            T sign = 1;
            T result = 0;
            for (auto i = 0; i < Size; i++) {
                result += mat[0][i] * Determinant(CreateMinor(mat, 0, i)) * sign;
                sign *= -1;
            }
            return result;
        }
    }

    template<typename T, size_t Size>
    [[nodiscard]] constexpr Mat<T, Size> CreateMatrixOfMinors(const Mat<T, Size>& mat) {
        auto result = mat;
        for (size_t i = 0; i < Size; i++) {
            for (size_t j = 0; j < Size; j++) {
                result[i][j] = Determinant(CreateMinor(mat, i, j));
            }
        }
        return result;
    }

    template<typename T, size_t Size>
    [[nodiscard]] constexpr Mat<T, Size> CreateMatrixOfCoefficients(const Mat<T, Size>& mat) {
        auto result = mat;
        T mul = 1;
        for (auto i = 0; i < Size; i++) {
            for (auto j = 0; j < Size; j++) {
                result[i][j] *= mul;
                mul *= -1;
            }
        }
        return result;
    }

    template<typename T, size_t Size>
    [[nodiscard]] constexpr Mat<T, Size> AdjoinMatrix(const Mat<T, Size>& mat) {
        auto result = mat;
        for (size_t i = 0; i < Size; i++) {
            for (size_t j = 0; j <= i; j++) {
                std::swap(result[i][j], result[j][i]);
            }
        }

        return result;
    }

    template<typename T, size_t Size>
    [[nodiscard]] constexpr Mat<T, Size> InvertMatrix(const Mat<T, Size>& mat) {
        if constexpr (Size == 2) {
            auto det = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
            if (det == 0) return mat;
            auto adj = Mat<T, 2>{ {{mat[1][1], -mat[0][1]}, {-mat[1][0], mat[0][0]}} };
            auto inv = 1.0 / det;
            return MultiplyMatrix(adj, inv);
        }
        else {
            auto minors = CreateMatrixOfMinors(mat);
            auto co = CreateMatrixOfCoefficients(minors);

            T determinant = 0;
            for (auto i = 0; i < Size; i++) {
                determinant += (co[i][0] * mat[i][0]);
            }
            if (determinant == 0) return mat;

            auto inv = 1.0 / determinant;
            auto adj = AdjoinMatrix(co);

            return MultiplyMatrix(adj, inv);
        }
    }

    bool RunMatrixTests();
}