#include "2021/d4_Bingo.h"

SOLUTION(2021, 4) {
    struct BingoCard {
        Constexpr::SmallMap<u32, RowCol> Numbers;
        Constexpr::SmallSet<RowCol> Marked;

        constexpr void Mark(u32 value) {
            if (Numbers.contains(value)) {
                Marked.insert(Numbers.at(value));
            }
        }

        constexpr bool HasBingo() const {
            for (size_t row = 0; row < 5; row++) {
                bool rowComplete = true;
                for (size_t col = 0; col < 5; col++) {
                    RowCol rc = { row, col };
                    if (!Marked.contains(rc)) {
                        rowComplete = false;
                        break;
                    }
                }
                if (rowComplete) return true;
            }
            for (size_t col = 0; col < 5; col++) {
                bool colComplete = true;
                for (size_t row = 0; row < 5; row++) {
                    RowCol rc = { row, col };
                    if (!Marked.contains(rc)) {
                        colComplete = false;
                        break;
                    }
                }
                if (colComplete) return true;
            }
            return false;
        }

        constexpr size_t CalculateScore(u32 lastNumber) const {
            size_t score = 0;
            for (const auto& [number, rc] : Numbers) {
                if (!Marked.contains(rc)) {
                    score += number;
                }
            }
            return score * lastNumber;
        }
    };

    constexpr BingoCard ParseCard(const auto& lines) {
        BingoCard result;
        for (size_t row = 0; row < lines.size(); row++) {
            auto split = Constexpr::Split(lines[row], " ");
            for (size_t col = 0; col < split.size(); col++) {
                RowCol rc = { row, col };
                u32 value;
                Constexpr::ParseNumber(split[col], value);
                result.Numbers[value] = rc;
            }
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto numbers = ParseLineAsNumbers<u32>(lines[0]);
        std::vector<BingoCard> cards;
        for (size_t i = 1; i < groups.size(); i++) {
            cards.push_back(ParseCard(groups[i]));
        }

        for (auto num : numbers) {
            for (auto& card : cards) {
                card.Mark(num);
                if (card.HasBingo()) {
                    return card.CalculateScore(num);
                }
            }
        }

        throw "Not Found";
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        auto numbers = ParseLineAsNumbers<u32>(lines[0]);
        std::vector<BingoCard> cards;
        for (size_t i = 1; i < groups.size(); i++) {
            cards.push_back(ParseCard(groups[i]));
        }

        size_t numIndex = 0;
        while (cards.size() > 1) {
            auto num = numbers[numIndex++];
            for (auto& card : cards) {
                card.Mark(num);
            }
            cards.erase(std::remove_if(cards.begin(), cards.end(), [](const BingoCard& card) {
                return card.HasBingo();
                }), cards.end());
        }
        auto lastCard = cards[0];
        while (!lastCard.HasBingo()) {
            lastCard.Mark(numbers[numIndex++]);
        }

        return lastCard.CalculateScore(numbers[numIndex - 1]);
    }
    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1",
            "",
            "22 13 17 11  0",
            " 8  2 23  4 24",
            "21  9 14 16  7",
            " 6 10  3 18  5",
            " 1 12 20 15 19",
            "",
            " 3 15  0  2 22",
            " 9 18 13 17  5",
            "19  8  7 25 23",
            "20 11 10 24  4",
            "14 21 16 12  6",
            "",
            "14 21 17 24  4",
            "10 16 15  9 19",
            "18  8 23 26 20",
            "22 11 13  6  5",
            " 2  0 12  3  7",
        };

        if (SolvePartOne(lines) != 4512) return false;
        if (SolvePartTwo(lines) != 1924) return false;
        return true;
    }
}