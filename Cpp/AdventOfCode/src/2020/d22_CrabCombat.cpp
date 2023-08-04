#include "2020/d22_CrabCombat.h"

SOLUTION(2020, 22) {
    constexpr std::vector<u32> ParseDeck(const std::vector<std::string>&lines) {
        std::vector<u32> result;
        for (size_t i = 1; i < lines.size(); i++) {
            u32 num;
            Constexpr::ParseNumber(lines[i], num);
            result.push_back(num);
        }
        return result;
    }

    constexpr void PlayRound(std::vector<u32>&d1, std::vector<u32>&d2) {
        while (!(d1.empty() || d2.empty())) {
            std::rotate(d1.begin(), d1.begin() + 1, d1.end());
            std::rotate(d2.begin(), d2.begin() + 1, d2.end());
            if (d1.back() < d2.back()) {
                d2.push_back(d1.back());
                d1.pop_back();
            }
            else {
                d1.push_back(d2.back());
                d2.pop_back();
            }
        }
    }

    constexpr size_t CalculateScore(const std::vector<u32>&deck) {
        size_t multiplier = 1;
        size_t result = 0;
        for (int i = static_cast<int>(deck.size()) - 1; i >= 0; i--) {
            result += deck[i] * multiplier++;
        }

        return result;
    }


    constexpr std::string GetSeenString(const std::vector<u32>&d1, const std::vector<u32>&d2) {
        auto lhs = Constexpr::JoinVec("", d1);
        auto rhs = Constexpr::JoinVec("", d2);
        return lhs + "|" + rhs;
    }

    constexpr bool PlaySubGame(std::vector<u32>&d1, std::vector<u32>&d2) {
        Constexpr::SmallSet<std::string> seen;
        while (!(d1.empty() || d2.empty())) {
            auto seenString = GetSeenString(d1, d2);
            if (seen.contains(seenString)) {
                return true;
            }
            seen.insert(seenString);
            std::rotate(d1.begin(), d1.begin() + 1, d1.end());
            std::rotate(d2.begin(), d2.begin() + 1, d2.end());

            auto c1 = d1.back();
            auto c2 = d2.back();
            d1.pop_back();
            d2.pop_back();

            if (c1 <= d1.size() && c2 <= d2.size()) {
                std::vector<u32> sub1 = std::vector<u32>(d1.begin(), d1.begin() + c1);
                std::vector<u32> sub2 = std::vector<u32>(d2.begin(), d2.begin() + c2);
                bool p1Wins = PlaySubGame(sub1, sub2);
                if (p1Wins) {
                    d1.push_back(c1);
                    d1.push_back(c2);
                }
                else {
                    d2.push_back(c2);
                    d2.push_back(c1);
                }
            }
            else {
                if (c1 < c2) {
                    d2.push_back(c2);
                    d2.push_back(c1);
                }
                else {
                    d1.push_back(c1);
                    d1.push_back(c2);
                }
            }
        }

        return d2.empty();
    }

    constexpr size_t Solve(const std::vector<std::string>& lines, auto Play) {
        auto groups = SplitInputIntoGroups(lines);
        auto d1 = ParseDeck(groups[0]);
        auto d2 = ParseDeck(groups[1]);

        Play(d1, d2);

        return d1.empty() ? CalculateScore(d2) : CalculateScore(d1);
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, PlayRound));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, PlaySubGame));
    }

    TESTS() {
        static_assert(CalculateScore({ 3, 2, 10, 6, 8, 5, 9, 4, 7, 1 }) == 306);
        
        std::vector<std::string> lines = {
            "Player 1:",
            "9","2","6","3","1",
            "",
            "Player 2:",
            "5","8","4","7","10",
        };

        if (PartOne(lines) != "306") return false;
        if (PartTwo(lines) != "291") return false;
        return true;
    }
}