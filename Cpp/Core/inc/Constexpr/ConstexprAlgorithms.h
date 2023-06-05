#pragma once

#include <vector>
#include <array>
#include <string>

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

    template<size_t Rows, size_t Cols>
    constexpr size_t GetIndex(size_t row, size_t col) {
        return row * Cols + col;
    }

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

    template<typename Collection>
    constexpr auto FindMax(const Collection& collection) {
        auto max = *collection.begin();
        for (const auto& elem : collection) {
            max = std::max(max, elem);
        }

        return max;
    }
}