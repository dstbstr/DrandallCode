#pragma once

#include "2015/d13_KnightsOfDinner.h"

SOLUTION(2015, 13) {
    using PreferenceMap = Constexpr::SmallMap<std::string, Constexpr::SmallMap<std::string, s32>>;

    constexpr void ParsePerson(std::string_view line, PreferenceMap& preferences, Constexpr::SmallSet<std::string>& allPeople) {
        //Alice would gain 2 happiness units by sitting next to Bob.
        auto s = Constexpr::Split(line, " ");
        auto lhs = std::string(s[0]);
        auto rhs = std::string(s.back());
        rhs = rhs.substr(0, rhs.size() - 1); //remove period at the end

        auto quality = std::string(s[2]);

        s32 amount;
        Constexpr::ParseNumber(s[3], amount);
        amount = amount * (quality == "gain" ? 1 : -1);
        allPeople.insert(lhs);
        allPeople.insert(rhs);
        preferences[lhs][rhs] = amount;
    }

    constexpr void AddSelf(PreferenceMap& preferences, Constexpr::SmallSet<std::string>& allPeople) {
        std::string self = "You";
        for (const auto& person : allPeople) {
            preferences[self][person] = 0;
            preferences[person][self] = 0;
        }

        allPeople.insert(self);
    }

    constexpr s32 CalculateHappiness(const std::vector<std::string>& people, const PreferenceMap& preferences) {
        s32 result = 0;

        for (auto i = 0; i < people.size() - 1; i++) {
            result += preferences.at(people[i]).at(people[i + 1]);
            result += preferences.at(people[i + 1]).at(people[i]);
        }

        result += preferences.at(people[0]).at(people.back());
        result += preferences.at(people.back()).at(people[0]);

        return result;
    }

    constexpr u32 FindBestSeating(const auto& lines, bool includeSelf) {
        PreferenceMap preferences;
        Constexpr::SmallSet<std::string> allPeople;

        for (const auto& line : lines) {
            ParsePerson(line, preferences, allPeople);
        }
        if (includeSelf) {
            AddSelf(preferences, allPeople);
        }

        auto arrangement = std::vector<std::string>{ allPeople.cbegin(), allPeople.cend() };
        std::sort(arrangement.begin(), arrangement.end());
        s32 result = -99999;

        do {
            result = std::max(result, CalculateHappiness(arrangement, preferences));
        } while (std::next_permutation(arrangement.begin(), arrangement.end()));

        return result;
    }

   PART_ONE() {
        return Constexpr::ToString(FindBestSeating(lines, false));
    }
    PART_TWO() {
        return Constexpr::ToString(FindBestSeating(lines, true));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Alice would gain 54 happiness units by sitting next to Bob.",
            "Alice would lose 79 happiness units by sitting next to Carol.",
            "Alice would lose 2 happiness units by sitting next to David.",
            "Bob would gain 83 happiness units by sitting next to Alice.",
            "Bob would lose 7 happiness units by sitting next to Carol.",
            "Bob would lose 63 happiness units by sitting next to David.",
            "Carol would lose 62 happiness units by sitting next to Alice.",
            "Carol would gain 60 happiness units by sitting next to Bob.",
            "Carol would gain 55 happiness units by sitting next to David.",
            "David would gain 46 happiness units by sitting next to Alice.",
            "David would lose 7 happiness units by sitting next to Bob.",
            "David would gain 41 happiness units by sitting next to Carol."
        };

        if (FindBestSeating(lines, false) != 330) return false;

        return true;
    }
}