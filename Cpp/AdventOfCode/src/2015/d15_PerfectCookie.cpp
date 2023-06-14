#pragma once

#include "2015/d15_PerfectCookie.h"
SOLUTION(2015, 15) {
    //Sugar: capacity 3, durability 0, flavor 0, texture -3, calories 2
    struct Ing {
        s32 Capacity = 0;
        s32 Durability = 0;
        s32 Flavor = 0;
        s32 Texture = 0;
        s32 Calories = 0;
    };

    constexpr Ing ParseIng(const std::string& line) {
        auto s0 = line.substr(line.find(':') + 2);
        auto s = Constexpr::Split(s0, ", ");
        Ing result;
        Constexpr::ParseNumber(Constexpr::Split(s[0], " ")[1], result.Capacity);
        Constexpr::ParseNumber(Constexpr::Split(s[1], " ")[1], result.Durability);
        Constexpr::ParseNumber(Constexpr::Split(s[2], " ")[1], result.Flavor);
        Constexpr::ParseNumber(Constexpr::Split(s[3], " ")[1], result.Texture);
        Constexpr::ParseNumber(Constexpr::Split(s[4], " ")[1], result.Calories);

        return result;
    }

    constexpr u32 CalculateScore(const std::vector<Ing>& ings, const std::vector<u32>& amounts) {
        s32 capacity = 0;
        s32 durability = 0;
        s32 flavor = 0;
        s32 texture = 0;

        for (size_t i = 0; i < ings.size(); i++) {
            capacity += ings[i].Capacity * amounts[i];
            durability += ings[i].Durability * amounts[i];
            flavor += ings[i].Flavor * amounts[i];
            texture += ings[i].Texture * amounts[i];
        }
        
        if (std::min({ capacity, durability, flavor, texture }) <= 0) return 0;
        return capacity * durability * flavor * texture;
    }

    constexpr s32 CalculateCalories(const std::vector<Ing>& ings, const std::vector<u32>& amounts) {
        s32 calories = 0;
        for (size_t i = 0; i < ings.size(); i++) {
            calories += ings[i].Calories * amounts[i];
        }
        return calories;
    }

    constexpr u32 Recurse(const std::vector<Ing>& ings, std::vector<u32> amounts, u32 totalSoFar, bool limitCalories) {
        if (amounts.size() == ings.size() - 1) {
            amounts.push_back(100 - totalSoFar);
            if (limitCalories) {
                auto calories = CalculateCalories(ings, amounts);
                if (calories != 500) {
                    return 0;
                }
            }
            return CalculateScore(ings, amounts);
        }

        amounts.push_back(1);
        u32 best = 0;
        for (u32 amount = 1; amount + totalSoFar < 100; amount++) {
            amounts.back() = amount;
            best = std::max(best, Recurse(ings, amounts, totalSoFar + amount, limitCalories));
        }
        
        return best;
    }

    PART_ONE() {
        auto ings = ParseLines<Ing>(lines, ParseIng);
        return Constexpr::ToString(Recurse(ings, {}, 0, false));
    }
    PART_TWO() {
        auto ings = ParseLines<Ing>(lines, ParseIng);
        return Constexpr::ToString(Recurse(ings, {}, 0, true));
    }
    TESTS() {
        auto ing = ParseIng("Sugar: capacity 3, durability 0, flavor 0, texture -3, calories 2");
        if (ing.Capacity != 3) return false;
        if (ing.Durability != 0) return false;
        if (ing.Flavor != 0) return false;
        if (ing.Texture != -3) return false;
        if (ing.Calories != 2) return false;

        std::vector<Ing> ings;
        ings.push_back(ParseIng("Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8"));
        ings.push_back(ParseIng("Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3"));

        std::vector<u32> amounts = { 44, 56 };
        if (CalculateScore(ings, amounts) != 62842880) return false;

        return true;
    }
}