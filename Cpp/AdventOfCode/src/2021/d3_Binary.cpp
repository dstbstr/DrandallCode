#include "2021/d3_Binary.h"
#include <bitset>

SOLUTION(2021, 3) {
    constexpr char FindMostCommon(const std::vector<std::string>&lines, size_t bit) {
        //size_t half = lines.size() / 2;
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

    constexpr void ReadValues(const std::vector<std::string>&lines, std::string & outLeft, std::string & outRight) {
        outLeft = "";
        outRight = "";

        for (auto i = 0; i < lines[0].size(); i++) {
            auto most = FindMostCommon(lines, i);
            auto least = most == '1' ? '0' : '1';
            outLeft.push_back(most);
            outRight.push_back(least);
        }
    }


    auto Part1(const std::vector<std::string>&lines) {
        std::string lhs, rhs;
        ReadValues(lines, lhs, rhs);

        return std::bitset<15>(lhs).to_ullong() * std::bitset<15>(rhs).to_ullong();
    }

    constexpr std::string FindPartTwo(const std::vector<std::string>&lines, bool keepMostCommon, size_t bit = 0) {
        auto mostCommon = FindMostCommon(lines, bit);
        auto leastCommon = mostCommon == '1' ? '0' : '1';
        std::vector<std::string> remaining;
        std::copy_if(lines.begin(), lines.end(), std::back_inserter(remaining), [&](const std::string& line) {
            if (keepMostCommon) {
                return line[bit] == mostCommon;
            }
            else {
                return line[bit] == leastCommon;
            }
            });
        if (remaining.size() == 1) {
            return remaining[0];
        }
        else {
            return FindPartTwo(remaining, keepMostCommon, bit + 1);
        }
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto oxygen = FindPartTwo(lines, true);
        auto co2 = FindPartTwo(lines, false);

        return std::bitset<15>(oxygen).to_ullong() * std::bitset<15>(co2).to_ullong();
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
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

        if (Part1(lines) != 198) return false;
        if (Part2(lines) != 230) return false;
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