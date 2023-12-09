#include "2022/d10_Crt.h"

SOLUTION(2022, 10) {
    constexpr void RunLine(std::string_view line, u32 & tick, std::vector<s32>&xValues) {
        auto x = xValues[tick];

        tick++;
        if (line[0] == 'n') {
            xValues.push_back(x);
            return;
        }

        tick++;
        xValues.push_back(x);

        auto split = Constexpr::Split(line, " ");
        s32 change;
        Constexpr::ParseNumber(split[1], change);

        xValues.push_back(x + change);
    }

    constexpr std::vector<s32> GetXValues(const auto& lines) {
        std::vector<s32> xValues{ 1 };
        u32 tick = 0;
        for (const auto& line : lines) {
            RunLine(line, tick, xValues);
        }

        return xValues;
    }

    constexpr u32 SumSignalStrength(const auto& lines) {
        auto xValues = GetXValues(lines);

        u32 result = 0;
        result += (20 * xValues[19]);
        result += (60 * xValues[59]);
        result += (100 * xValues[99]);
        result += (140 * xValues[139]);
        result += (180 * xValues[179]);
        result += (220 * xValues[219]);
        return result;
    }

    constexpr std::string BuildCrtString(const auto& lines) {
        auto xValues = GetXValues(lines);
        std::string result = "\n";

        for (auto row = 0; row < 6; row++) {
            for (auto col = 0; col < 40; col++) {
                auto pos = (row * 40 + col);
                auto x = xValues[pos];
                bool lit = x > col - 2 && x < col + 2;
                result += (lit ? '#' : '.');
            }
            result += "\n";
        }

        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(SumSignalStrength(lines));
    }

    PART_TWO() {
        return BuildCrtString(lines);
    }

    TESTS() {
        return true;
    }
}