#include "2021/d9_LavaTube.h"
#include "Algorithms/FloodFill.h"

SOLUTION(2021, 9) {
    std::vector<u32> ParseLine(std::string_view line) {
        std::vector<u32> result;
        for (auto c : line) {
            result.push_back(c - '0');
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        size_t result = 0;
        auto nums = ParseLines(lines, ParseLine);
        for (size_t row = 0; row < nums.size(); row++) {
            for (size_t col = 0; col < nums[row].size(); col++) {
                RowCol rc{ row, col };
                auto neighbors = GetDirectNeighbors(rc, limits);
                auto min = std::reduce(neighbors.begin(), neighbors.end(), neighbors[0], [&nums](RowCol lhs, RowCol rhs) {
                    return nums[lhs.Row][lhs.Col] < nums[rhs.Row][rhs.Col] ? lhs : rhs;
                    });
                if (nums[rc.Row][rc.Col] < nums[min.Row][min.Col]) {
                    result += nums[rc.Row][rc.Col];
                    result++;
                }
            }
        }

        return result;
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        auto nums = ParseLines(lines, ParseLine);
        std::vector<size_t> sizes;

        auto n = [&](const RowCol& pos) {
            auto neighbors = GetDirectNeighbors(pos, limits);
            std::vector<RowCol> result;
            std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(result), [&](const RowCol& neighbor) {
                return nums[neighbor.Row][neighbor.Col] != 9;
                });
            return result;
            };

        for (size_t row = 0; row < nums.size(); row++) {
            for (size_t col = 0; col < nums[row].size(); col++) {
                if (nums[row][col] == 9) continue;
                RowCol rc{ row, col };

                auto region = FloodFill<RowCol>(rc, n);
                sizes.push_back(region.size());
                for (const auto& pos : region) {
                    nums[pos.Row][pos.Col] = 9;
                }
            }
        }

        std::sort(sizes.begin(), sizes.end());
        size_t result = 1;
        for (auto i = 0; i < 3; i++) {
            result *= sizes.back();
            sizes.pop_back();
        }
        return result;
    }
    
    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "2199943210",
            "3987894921",
            "9856789892",
            "8767896789",
            "9899965678"
        };

        if (SolvePartOne(lines) != 15) return false;
        if (SolvePartTwo(lines) != 1134) return false;
        return true;
    }
}