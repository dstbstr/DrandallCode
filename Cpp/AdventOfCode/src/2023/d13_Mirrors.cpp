#include "2023/d13_Mirrors.h"

SOLUTION(2023, 13) {
    constexpr size_t CountChanges(const std::string & lhs, const std::string & rhs) {
        size_t result = 0;
        for (auto i = 0; i < lhs.size(); i++) {
            result += lhs[i] != rhs[i];
        }
        return result;
    }
    
    constexpr size_t FindHorizontalMirror(const std::vector<std::string>&image, size_t requiredChanges) {
        auto IsMirror = [&](size_t row) {
            size_t actualDifferences = 0;
            for (auto i = 0; i <= row; i++) {
                if (i + row + 1 == image.size()) return actualDifferences == requiredChanges;
                actualDifferences += CountChanges(image[row + i + 1], image[row - i]);
                if (actualDifferences > requiredChanges) return false;
            }
            return actualDifferences == requiredChanges;
            };

        for (auto row = 0; row < image.size() - 1; row++) {
            if (IsMirror(row)) return row + 1;
        }
        return 0;
    }

    constexpr size_t FindVerticalMirror(const std::vector<std::string>& image, size_t requiredChanges) {
        auto GetCol = [&](size_t col) -> std::string {
            std::string result;
            result.reserve(image.size());
            for (const auto& line : image) {
                result.push_back(line[col]);
            }
            return result;
        };
        auto IsMirror = [&](size_t col) -> bool {
            size_t actualDifferences = 0;
            for (auto i = 0; i <= col; i++) {
                if (i + col + 1 == image[0].size()) return actualDifferences == requiredChanges;
                auto left = GetCol(col - i);
                auto right = GetCol(col + i + 1);
                actualDifferences += CountChanges(left, right);
                if (actualDifferences > requiredChanges) return false;
            }
            return actualDifferences == requiredChanges;
            };

        for (auto col = 0; col < image[0].size() - 1; col++) {
            if (IsMirror(col)) return col + 1;
        }
        return 0;
    }

    constexpr std::pair<size_t, size_t> FindMirror(const std::vector<std::string>& image, size_t requiredChanges) {
        std::pair<size_t, size_t> result{ 0, 0 };
        
        result.first = FindHorizontalMirror(image, requiredChanges);
        if (result.first > 0) return result;

        result.second = FindVerticalMirror(image, requiredChanges);
        return result;
    }

    constexpr size_t Solve(const std::vector<std::string>& lines, size_t requiredChanges) {
        auto images = SplitInputIntoGroups(lines);
        std::pair<size_t, size_t> result;
        for (const auto& image : images) {
            auto [rows, cols] = FindMirror(image, requiredChanges);
            result.first += rows;
            result.second += cols;
        }

        return (result.first * 100) + result.second;
    }
    PART_ONE() {
        return Constexpr::ToString(Solve(lines, 0));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, 1));
    }

    TESTS() {
        std::vector<std::string> example = {
            "#.##..##.",
            "..#.##.#.",
            "##......#",
            "##......#",
            "..#.##.#.",
            "..##..##.",
            "#.#.##.#.",
            "",
            "#...##..#",
            "#....#..#",
            "..##..###",
            "#####.##.",
            "#####.##.",
            "..##..###",
            "#....#..#"
        };

        if (PartOne(example) != "405") return false;
        return true;
    }
}