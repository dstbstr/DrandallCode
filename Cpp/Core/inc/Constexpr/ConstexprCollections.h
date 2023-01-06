#pragma once

#include <vector>
#include <array>

namespace Constexpr {
    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, Collection&& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(std::string&& delimiter, const Collection& input) {
        std::string result;
        bool first = true;
        for (const auto& elem : input) {
            if (first) first = false;
            else result += delimiter;
            result += elem;
        }

        return result;
    }

    template<typename Collection>
    constexpr std::string JoinVec(char&& delimiter, Collection const& input) {
        return JoinVec(std::string(1, delimiter), input);
    }

    static_assert(JoinVec(" ", std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec(' ', std::vector<std::string>{ "a", "b", "c" }) == "a b c");
    static_assert(JoinVec("Hello", std::vector<std::string>{ "a", "b", "c" }) == "aHellobHelloc");

    template<size_t Rows, size_t Cols>
    constexpr size_t GetIndex(size_t row, size_t col) {
        return row * Cols + col;
    }

    static_assert(GetIndex<3, 3>(0, 0) == 0);
    static_assert(GetIndex<3, 3>(2, 2) == 8);

    template<size_t Rows, size_t Cols>
    constexpr void GetRowCol(size_t index, size_t& row, size_t& col) {
        row = index / Cols;
        col = index % Cols;
    }

    template<typename Collection>
    constexpr auto FindMin(const Collection& collection) {
        auto min = *collection.begin();
        for (const auto& elem : collection) {
            min = std::min(min, elem);
        }

        return min;
    }

    static_assert(FindMin(std::vector<u32>{ 0, 1, 1 }) == 0);
    static_assert(FindMin(std::vector<u32>{1, 2, 0}) == 0);
    static_assert(FindMin(std::vector<u32>{1}) == 1);

    template<typename Collection>
    constexpr auto FindMax(const Collection& collection) {
        auto max = *collection.begin();
        for (const auto& elem : collection) {
            max = std::max(max, elem);
        }

        return max;
    }

    static_assert(FindMax(std::vector<u32>{1, 2, 3}) == 3);
    static_assert(FindMax(std::array<u32, 3>{3, 2, 1}) == 3);
    static_assert(FindMax(std::vector<s32>{-1}) == -1);

    template<typename Func>
    constexpr void ApplyToRowCol(const auto& matrix, Func func) {
        for (size_t row = 0; row < matrix.size(); row++) {
            for (size_t col = 0; col < matrix[row].size(); col++) {
                func(row, col);
            }
        }
    }

    template<typename Matrix>
    constexpr Matrix FlipX(const Matrix& matrix) {
        Matrix result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToRowCol(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - row][col];
            });
        return result;
    }

    static_assert(FlipX(std::vector<std::vector<int>>{ {1, 2}, { 3, 4 }}) == std::vector<std::vector<int>> { {3, 4}, { 1, 2 }});
    static_assert(FlipX(std::vector<std::vector<int>>{ {1, 2, 3}, { 4, 5, 6 }, { 7, 8, 9 }}) == std::vector<std::vector<int>>{ {7, 8, 9}, { 4, 5, 6 }, { 1, 2, 3 }});

    template<typename Matrix>
    constexpr Matrix FlipY(const Matrix& matrix) {
        Matrix result = matrix;
        size_t n = matrix[0].size() - 1;
        ApplyToRowCol(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[row][n - col];
            });

        return result;
    }

    static_assert(FlipY(std::vector<std::vector<int>>{ {1, 2}, { 3, 4 }}) == std::vector<std::vector<int>>{ {2, 1}, { 4, 3 }});
    static_assert(FlipY(std::vector<std::vector<int>>{ {1, 2, 3}, { 4, 5, 6 }, { 7, 8, 9 }}) == std::vector<std::vector<int>>{ {3, 2, 1}, { 6, 5, 4 }, { 9, 8, 7 }});

    template<typename Matrix>
    constexpr Matrix Rotate(const Matrix& matrix) {
        Matrix result = matrix;
        size_t n = matrix.size() - 1;
        ApplyToRowCol(matrix, [&](size_t row, size_t col) {
            result[row][col] = matrix[n - col][row];
            });

        return result;
    }

    static_assert(Rotate(std::vector<std::vector<int>>{
        {1, 2, 3},
        { 4, 5, 6 },
        { 7, 8, 9 }}) == std::vector<std::vector<int>> {
        {7, 4, 1},
        { 8, 5, 2 },
        { 9, 6, 3 }
    });

}