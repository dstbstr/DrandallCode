#include "2022/d8_TreeTop.h"

SOLUTION(2022, 8) {
    template<size_t Rows, size_t Cols>
    constexpr bool IsTreeVisible(const std::array<std::array<u8, Cols>, Rows>&forest, size_t treeRow, size_t treeCol) {
        u8 treeHeight = forest[treeRow][treeCol];
        bool visible = true;
        for (size_t row = 0; row < treeRow; row++) {
            if (forest[row][treeCol] >= treeHeight) {
                visible = false;
                break;
            }
        }
        if (visible) return true;

        visible = true;
        for (size_t row = Rows - 1; row > treeRow; row--) {
            if (forest[row][treeCol] >= treeHeight) {
                visible = false;
                break;
            }
        }
        if (visible) return true;

        visible = true;
        for (size_t col = 0; col < treeCol; col++) {
            if (forest[treeRow][col] >= treeHeight) {
                visible = false;
                break;
            }
        }
        if (visible) return true;

        visible = true;
        for (size_t col = Cols - 1; col > treeCol; col--) {
            if (forest[treeRow][col] >= treeHeight) {
                visible = false;
                break;
            }
        }
        return visible;
    }

    template<size_t Rows, size_t Cols>
    constexpr u32 VisibleInForest(const std::array<std::array<u8, Cols>, Rows> &forest) {
        u32 result = ((Rows + Cols) * 2) - 4;
        for (size_t row = 1; row < Rows - 1; row++) {
            for (size_t col = 1; col < Cols - 1; col++) {
                if (IsTreeVisible(forest, row, col)) {
                    result++;
                }
            }
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr u32 GetScenicScore(const std::array<std::array<u8, Cols>, Rows>&forest, size_t treeRow, size_t treeCol) {
        u32 upScore = 0;
        u32 downScore = 0;
        u32 leftScore = 0;
        u32 rightScore = 0;

        auto treeHeight = forest[treeRow][treeCol];

        for (s32 row = static_cast<s32>(treeRow - 1); row >= 0; row--) {
            downScore++;
            if (treeHeight <= forest[row][treeCol]) break;
        }
        for (auto row = treeRow + 1; row < Rows; row++) {
            upScore++;
            if (treeHeight <= forest[row][treeCol]) break;
        }
        for (s32 col = static_cast<s32>(treeCol - 1); col >= 0; col--) {
            leftScore++;
            if (treeHeight <= forest[treeRow][col]) break;
        }
        for (auto col = treeCol + 1; col < Cols; col++) {
            rightScore++;
            if (treeHeight <= forest[treeRow][col]) break;
        }

        return upScore * downScore * leftScore * rightScore;
    }

    template<size_t Rows, size_t Cols>
    constexpr u32 FindBestScenicScore(const std::array<std::array<u8, Cols>, Rows>&forest) {
        u32 best = 0;
        for (size_t row = 0; row < Rows - 1; row++) {
            for (size_t col = 0; col < Cols - 1; col++) {
                best = std::max(best, GetScenicScore(forest, row, col));
            }
        }

        return best;
    }

    constexpr size_t Solve(const auto& lines, auto Func) {
        std::array<std::array<u8, 99>, 99> forest;
        for (auto row = 0; row < 99; row++) {
            for (auto col = 0; col < 99; col++) {
                forest[row][col] = lines[row][col] - '0';
            }
        }

        return Func(forest);
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Lines, VisibleInForest<99, 99>));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Lines, FindBestScenicScore<99, 99>));
    }

    TESTS() {
        static_assert(VisibleInForest<5, 5>({ {
           {3, 0, 3, 7, 3},
           {2, 5, 5, 1, 2},
           {6, 5, 3, 3, 2},
           {3, 3, 5, 4, 9},
           {3, 5, 3, 9, 0}
           } }) == 21);
        
        static_assert(GetScenicScore<5, 5>({ {
           {3, 0, 3, 7, 3},
           {2, 5, 5, 1, 2},
           {6, 5, 3, 3, 2},
           {3, 3, 5, 4, 9},
           {3, 5, 3, 9, 0}
           } }, 1, 2) == 4);

        static_assert(GetScenicScore<5, 5>({ {
            {3, 0, 3, 7, 3},
            {2, 5, 5, 1, 2},
            {6, 5, 3, 3, 2},
            {3, 3, 5, 4, 9},
            {3, 5, 3, 9, 0}
            } }, 3, 2) == 8);
        
        static_assert(FindBestScenicScore<5, 5>({ {
            {3, 0, 3, 7, 3},
            {2, 5, 5, 1, 2},
            {6, 5, 3, 3, 2},
            {3, 3, 5, 4, 9},
            {3, 5, 3, 9, 0}
            } }) == 8);

        return true;
    }
}