#include "2023/d4_Scratch.h"

SOLUTION(2023, 4) {
    struct Card {
        u32 Id = 0;
        Constexpr::SmallSet<u32> Winners;
        Constexpr::SmallSet<u32> Numbers;

        constexpr u32 CountMatches() const {
            u32 matches = 0;
            for (auto num : Winners) {
                matches += Numbers.contains(num);
            }
            return matches;
        }

        constexpr u32 Score() const {
            auto matches = CountMatches();
            if (matches == 0) return 0;

            return 1 << (matches - 1);
        }
    };

    //Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    constexpr Card ParseCard(const std::string& line) {
        auto s1 = Constexpr::Split(line, ": ");
        auto cardId = Constexpr::Split(s1[0], " ")[1];
        auto groups = Constexpr::Split(s1[1], " | ");
        auto winners = Constexpr::Split(groups[0], " ");
        auto numbers = Constexpr::Split(groups[1], " ");

        Card result;
        Constexpr::ParseNumber(cardId, result.Id);
        for (const auto str : winners) {
            u32 val;
            Constexpr::ParseNumber(str, val);
            result.Winners.insert(val);
        }
        for (const auto str : numbers) {
            u32 val;
            Constexpr::ParseNumber(str, val);
            result.Numbers.insert(val);
        }

        return result;
    }
    PART_ONE() {
        auto cards = ParseLines(lines, ParseCard);
        auto result = std::accumulate(cards.begin(), cards.end(), 0u, [](u32 running, const Card& card) {
            return running + card.Score();
            });

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto cards = ParseLines(lines, ParseCard);
        std::vector<u32> multipliers(cards.size() + 1, 1);
        for (const auto& card : cards) {
            auto matches = card.CountMatches();
            for (size_t i = 0; i < matches; i++) {
                multipliers[card.Id + i + 1 ] += multipliers[card.Id];
            }
        }

        return Constexpr::ToString(std::accumulate(multipliers.begin(), multipliers.end(), -1));
    }

    TESTS() {
        auto card = ParseCard("Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 5");
        if (card.Id != 1) return false;
        if (card.Winners.size() != 5) return false;
        if (card.Numbers.size() != 8) return false;

        if (card.Score() != 8) return false;

        std::vector<std::string> example = {
            "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
            "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19",
            "Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1",
            "Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83",
            "Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36",
            "Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11"
        };

        if (PartTwo(example) != "30") return false;
        return true;
    }
}