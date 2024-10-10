#include "2021/d3_Binary.h"

//TODO: Change this to a constexpr solution
#include <bitset>

SOLUTION(2021, 3) {
    constexpr char FindMostCommon(const auto& lines, size_t bit) {
        size_t oneCount = 0;
        for (const auto& line : lines) {
            oneCount += line[bit] == '1';
        }
        if (oneCount + oneCount >= lines.size()) {
            return '1';
        }
        else {
            return '0';
        }
    }

    constexpr void ReadValues(const auto& lines, std::string & outLeft, std::string & outRight) {
        outLeft = "";
        outRight = "";

        for (auto i = 0; i < lines[0].size(); i++) {
            auto most = FindMostCommon(lines, i);
            auto least = most == '1' ? '0' : '1';
            outLeft.push_back(most);
            outRight.push_back(least);
        }
    }


    constexpr std::string FindPartTwo(const auto& lines, bool keepMostCommon, size_t bit = 0) {
        auto mostCommon = FindMostCommon(lines, bit);
        auto leastCommon = mostCommon == '1' ? '0' : '1';
        std::vector<std::string_view> remaining;
        std::copy_if(lines.begin(), lines.end(), std::back_inserter(remaining), [&](std::string_view line) {
            if (keepMostCommon) {
                return line[bit] == mostCommon;
            }
            else {
                return line[bit] == leastCommon;
            }
            });
        if (remaining.size() == 1) {
            return std::string(remaining[0]);
        }
        else {
            return FindPartTwo(remaining, keepMostCommon, bit + 1);
        }
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        std::string lhs, rhs;
        ReadValues(lines, lhs, rhs);

        return std::bitset<15>(lhs).to_ullong() * std::bitset<15>(rhs).to_ullong();
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto oxygen = FindPartTwo(lines, true);
        auto co2 = FindPartTwo(lines, false);

        return std::bitset<15>(oxygen).to_ullong() * std::bitset<15>(co2).to_ullong();
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "00100",
            "11110",
            "10110",
            "10111",
            "10101",
            "01111",
            "00111",
            "11100",
            "10000",
            "11001",
            "00010",
            "01010"
        };

        if (SolvePartOne(lines) != 198) return false;
        if (SolvePartTwo(lines) != 230) return false;
        return true;
    }
}