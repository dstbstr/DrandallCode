#pragma once

#include "2015/d16_AuntSue.h"

SOLUTION(2015, 16) {

    using Sue = Constexpr::SmallMap<std::string, u32>;

    constexpr Sue GetTargetSue() {
        Sue result;
        result["children"] = 3;
        result["cats"] = 7;
        result["samoyeds"] = 2;
        result["pomeranians"] = 3;
        result["akitas"] = 0;
        result["vizslas"] = 0;
        result["goldfish"] = 5;
        result["trees"] = 3;
        result["cars"] = 2;
        result["perfumes"] = 1;

        return result;
    }
    constexpr Sue ParseSue(std::string_view line) {
        //Sue 1: goldfish: 6, trees: 9, akitas: 0

        auto propStart = line.find(':');
        auto rhs = line.substr(propStart + 2);
        auto s1 = Constexpr::Split(rhs, ", ");

        Sue result;
        for (const auto& pair : s1) {
            auto s2 = Constexpr::Split(pair, ": ");
            auto prop = std::string(s2[0]);
            auto valStr = std::string(s2[1]);
            u32 val;
            Constexpr::ParseNumber(valStr, val);
            result[prop] = val;
        }

        return result;
    }

    constexpr bool IsExactMatch(const Sue & sue) {
        const auto TargetSue = GetTargetSue();
        for (const auto& [key, value] : TargetSue) {
            if (sue.contains(key) && sue.at(key) != value) return false;
        }

        return true;
    }

    constexpr bool IsRangeMatch(const Sue & sue) {
        const auto TargetSue = GetTargetSue();
        for (const auto& [key, value] : TargetSue) {
            if (!sue.contains(key)) continue;

            if (key == "cats" || key == "trees") {
                if (sue.at(key) <= value) return false;
            }
            else if (key == "pomeranians" || key == "goldfish") {
                if (sue.at(key) >= value) return false;
            }
            else {
                if (sue.at(key) != value) return false;
            }
        }
        return true;
    }

    constexpr size_t Solve(const auto& lines, auto match) {
        auto sues = ParseLines(lines, ParseSue);
        for (size_t i = 0; i < sues.size(); i++) {
            if (match(sues[i])) return i + 1;
        }
        return 0;
    }
    PART_ONE() {
        return Constexpr::ToString(Solve(lines, IsExactMatch));
    }
    PART_TWO() {
        return Constexpr::ToString(Solve(lines, IsRangeMatch));
    }

    TESTS() {
        return true;
    }
}