#pragma once

#include "Common.h"

struct Ing {
    s32 Capacity = 0;
    s32 Durability = 0;
    s32 Flavor = 0;
    s32 Texture = 0;
    s32 Calories = 0;
};

Ing ParseIng(const std::string& line) {
    static auto re = std::regex(R"((-?\d+)[^-\d]+(-?\d+)[^-\d]+(-?\d+)[^-\d]+(-?\d+)[^-\d]+(-?\d+))");

    auto match = std::smatch{};
    std::regex_search(line, match, re);

    s32 capacity;
    s32 durability;
    s32 flavor;
    s32 texture;
    s32 calories;

    ParseNumber(match[1].str(), capacity);
    ParseNumber(match[2].str(), durability);
    ParseNumber(match[3].str(), flavor);
    ParseNumber(match[4].str(), texture);
    ParseNumber(match[5].str(), calories);

    return {capacity, durability, flavor, texture, calories};
}

template<size_t N>
constexpr u32 CalculateScore(std::array<std::pair<Ing, u32>, N> ings) {
    s32 capacity = 0;
    for(const auto[ing, count] : ings) {
        capacity += ing.Capacity * count;
    }
    if(capacity <= 0) return 0;

    s32 durability = 0;
    for(const auto[ing, count] : ings) {
        durability += ing.Durability * count;
    }
    if(durability <= 0) return 0;

    s32 flavor = 0;
    for(const auto[ing, count] : ings) {
        flavor += ing.Flavor * count;
    }
    if(flavor <= 0) return 0;

    s32 texture = 0;
    for(const auto[ing, count] : ings) {
        texture += ing.Texture * count;
    }
    if(texture <= 0) return 0;

    return capacity * durability * flavor * texture;
}

template<size_t N>
constexpr u32 CalculateCalories(std::array<std::pair<Ing, u32>, N> ings) {
    s32 calories = 0;
    for(const auto[ing, count] : ings) {
        calories += ing.Calories * count;
    }

    return static_cast<u32>(calories);
}

u32 FindBestScore(std::vector<Ing> ings) {
    auto attempt = std::array<std::pair<Ing, u32>, 4>{};

    u32 bestScore = 0;

    for(auto i1 = 1; i1 < 98; i1++) {
        for(auto i2 = 1; i2 < 98 - i1; i2++) {
            for(auto i3 = 1; i3 < 98 - i1 - i2; i3++) {
                attempt[0] = std::make_pair(ings[0], i1);
                attempt[1] = std::make_pair(ings[1], i2);
                attempt[2] = std::make_pair(ings[2], i3);
                attempt[3] = std::make_pair(ings[3], 100 - i1 - i2 -i3);

                if(CalculateCalories(attempt) == 500) {
                    auto score = CalculateScore(attempt);
                    bestScore = std::max(bestScore, score);
                }
            }
        }
    }

    return bestScore;
}

#define ASSERT(cond) if(!(cond)) return false;
bool RunTests() {
    std::array<std::pair<Ing, u32>, 2> arr;
    arr[0] = std::make_pair<Ing, u32>({-1, -2, 6, 3, 8}, 44);
    arr[1] = std::make_pair<Ing, u32>({2, 3, -2, -1, 3}, 56);

    ASSERT(62842880 == CalculateScore(arr));

    auto ing = ParseIng("Sugar: capacity 3, durability 0, flavor 0, texture -3, calories 2");
    ASSERT(ing.Capacity == 3);
    ASSERT(ing.Durability == 0);
    ASSERT(ing.Flavor == 0);
    ASSERT(ing.Texture == -3);
    ASSERT(ing.Calories == 2);

    return true;
}
#undef ASSERT
