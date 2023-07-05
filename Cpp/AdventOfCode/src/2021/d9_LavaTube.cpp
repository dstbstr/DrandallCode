#include "2021/d9_LavaTube.h"
#include "Algorithms/FloodFill.h"

SOLUTION(2021, 9) {
    std::vector<std::vector<u32>> ParseInput(const std::vector<std::string>&lines) {
        std::vector<std::vector<u32>> result;
        for (const auto& line : lines) {
            std::vector<u32> currentLine;
            for (auto c : line) {
                currentLine.push_back(c - '0');
            }
            result.push_back(currentLine);
        }
        return result;
    }

    auto Part1(const std::vector<std::string>&lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        size_t result = 0;
        auto nums = ParseInput(lines);
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

    auto Part2(const std::vector<std::string>&lines) {
        RowCol limits = { lines.size() - 1, lines[0].size() - 1 };
        auto nums = ParseInput(lines);
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
                std::vector<RowCol> region;

                FloodFill<RowCol>(rc, region, n);
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

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "2199943210",
            "3987894921",
            "9856789892",
            "8767896789",
            "9899965678"
        };

        if (Part1(lines) != 15) return false;
        if (Part2(lines) != 1134) return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}