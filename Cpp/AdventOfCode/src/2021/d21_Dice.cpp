#include "2021/d21_Dice.h"

SOLUTION(2021, 21) {
    constexpr bool Roll(size_t & dieRolls, size_t & dieValue, size_t & pos, size_t & score) {
        auto spaces = (dieValue + 1) * 3;
        pos = (pos + spaces) % 10;
        score += pos + 1;
        dieRolls += 3;
        dieValue += 3;
        if (dieValue > 100) dieValue -= 100;
        return score >= 1000;
    }

    constexpr auto Part1(size_t p1Pos, size_t p2Pos) {
        //let's make these zero based
        p1Pos--;
        p2Pos--;

        size_t dieRolls = 0;
        size_t dieValue = 1;
        size_t p1Score = 0;
        size_t p2Score = 0;
        while (true) {
            if (Roll(dieRolls, dieValue, p1Pos, p1Score)) break;
            if (Roll(dieRolls, dieValue, p2Pos, p2Score)) break;
        }

        return std::min(p1Score, p2Score) * dieRolls;
    }

    static_assert(Part1(4, 8) == 739785);
    static_assert(Part1(6, 10) == 853776);

    //s1, s2, p1, p2
    using Cache = std::array<std::array<std::array<std::array<std::pair<size_t, size_t>, 10>, 10>, 21>, 21>;
    //using Cache = std::array<std::array<std::pair<size_t, size_t>, 21>, 21>;
    constexpr auto Missing = std::make_pair<size_t, size_t>(0, 0);

    constexpr std::vector<size_t> GetPositions(size_t current) {
        return { (current + 1) % 10, (current + 2) % 10, (current + 3) % 10 };
    }

    struct Player {
        size_t Score = 0;
        size_t Pos = 0;
        Cache* Cache = nullptr;
    };

    constexpr std::array<std::pair<size_t, size_t>, 7> SpaceCounts = { {
        {3, 1},
        {4, 3},
        {5, 6},
        {6, 7},
        {7, 6},
        {8, 3},
        {9, 1}
    } };

    constexpr std::vector<std::pair<Player, size_t>> GetNext(const Player & p) {
        std::vector<std::pair<Player, size_t>> result;
        for (auto& sc : SpaceCounts) {
            auto next = p;
            next.Pos = (p.Pos + sc.first) % 10;
            next.Score += next.Pos + 1;
            result.push_back(std::make_pair(next, sc.second));
        }

        return result;
    }

    //roll die 3 times, add score once
    std::pair<size_t, size_t> Recurse(Player p1, Player p2, bool isP1Turn) {
        auto& cache = isP1Turn ? *p1.Cache : *p2.Cache;
        auto& seen = cache[p1.Score][p2.Score][p1.Pos][p2.Pos];

        if (seen == Missing) {
            std::pair<size_t, size_t> wins{};
            auto nextPlayers = GetNext(isP1Turn ? p1 : p2);
            for (const auto& [p, count] : nextPlayers) {
                if (p.Score >= 21) {
                    isP1Turn ? wins.first += count : wins.second += count;
                }
                else {
                    auto r = (isP1Turn ? Recurse(p, p2, false) : Recurse(p1, p, true));
                    wins.first += (r.first * count);
                    wins.second += (r.second * count);
                }
            }

            seen = wins;
        }

        return seen;
    }

    auto Part2(size_t p1Pos, size_t p2Pos) {
        p1Pos--;
        p2Pos--;
        auto p1Cache = std::make_unique<Cache>();
        auto p2Cache = std::make_unique<Cache>();

        Player p1{ 0, p1Pos, p1Cache.get() };
        Player p2{ 0, p2Pos, p2Cache.get() };
        auto result = Recurse(p1, p2, true);
        return std::max(result.first, result.second);
    }

    std::string Run(const std::vector<std::string>&) {
        //return Constexpr::ToString(Part1(6, 10));
        return Constexpr::ToString(Part2(6, 10));
    }

    bool RunTests() {
        if (Part1(4, 8) != 739785) return false;
        if (Part2(4, 8) != 444356092776315u) return false;
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