#include "2022/d19_GatherOre.h"
SOLUTION(2022, 19) {

    using Entry = std::array<u32, 4>;
    using Bp = std::vector<std::pair<Entry, Entry>>;

    constexpr Bp ParseBlueprint(std::string_view line) {
        Bp bp;
        auto s = Constexpr::Split(line, " ");

        u32 a, b, c, d, e, f;
        Constexpr::ParseNumber(s[6], a);
        Constexpr::ParseNumber(s[12], b);
        Constexpr::ParseNumber(s[18], c);
        Constexpr::ParseNumber(s[21], d);
        Constexpr::ParseNumber(s[27], e);
        Constexpr::ParseNumber(s[30], f);

        bp.push_back(std::make_pair(Entry{ 0, 0, 0, a }, Entry{ 0, 0, 0, 1 }));
        bp.push_back(std::make_pair(Entry{ 0, 0, 0, b }, Entry{ 0, 0, 1, 0 }));
        bp.push_back(std::make_pair(Entry{ 0, 0, d, c }, Entry{ 0, 1, 0, 0 }));
        bp.push_back(std::make_pair(Entry{ 0, f, 0, e }, Entry{ 1, 0, 0, 0 }));
        bp.push_back(std::make_pair(Entry{ 0, 0, 0, 0 }, Entry{ 0, 0, 0, 0 }));

        return bp;
    }

    constexpr bool all(const Entry& cost, const Entry& have) {
        for (auto i = 0; i < cost.size(); i++) {
            if (cost[i] > have[i]) return false;
        }
        return true;
    }

    constexpr Entry operator+(const Entry& lhs, const Entry& rhs) {
        return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] };
    }
    constexpr Entry operator-(const Entry& lhs, const Entry& rhs) {
        return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] };
    }

    constexpr u32 FindBfs(const Bp& bp, u32 minutes) {
        size_t toKeep = 1000;
        std::vector<std::pair<Entry, Entry>> todo{ std::make_pair(Entry{0, 0, 0, 0}, Entry{0, 0, 0, 1}) };
        std::vector<std::pair<Entry, Entry>> next;

        for (u32 minute = 1; minute <= minutes; minute++) {
            for (auto [have, make] : todo) {
                for (auto [cost, more] : bp) {
                    if (all(cost, have)) {
                        next.push_back(std::make_pair(have + make - cost, make + more));
                    }
                }
            }
            if (next.size() > toKeep) {
                std::sort(next.begin(), next.end(), [](const auto& lhs, const auto& rhs) {
                    auto score1 = lhs.first + lhs.second + lhs.second;
                    auto score2 = rhs.first + rhs.second + rhs.second;
                    return score1 < score2;
                    });
                todo.clear();
                std::copy_n(next.rbegin(), toKeep, std::back_inserter(todo));
            }
            else {
                todo = next;
            }
            next.clear();
        }
        u32 result = 0;
        for (const auto& [have, make] : todo) {
            result = std::max(result, have[0]);
        }
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto bps = ParseLines(lines, ParseBlueprint);
        size_t result = 0;
        for (size_t i = 0; i < bps.size(); i++) {
            result += (FindBfs(bps[i], 24)) * (i + 1);
        }
        return result;
    }
    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    PART_TWO() {
        auto bps = ParseLines(Lines, ParseBlueprint);
        size_t result = 1;
        for (size_t i = 0; i < 3; i++) {
            result *= FindBfs(bps[i], 32);
        }

        return Constexpr::ToString(result);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.",
            "Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian."
        };

        //auto bps = ParseLines(lines, ParseBp);
        //auto bps = ParseLines(lines, ParseBlueprint);

        //auto bp = ParseBlueprint(lines[0]);
        //if (Needs(15 * Robots[Clay], Clay, bp)) return false;
        //if (CanAfford(5 * Robots[Geo], Geo, bp)) return false;

        if (SolvePartOne(lines) != 33) return false;

        /*
        if (FindMostGeodes(bps[0], 24) != 9) return false;
        if (FindMostGeodes(bps[1], 24) != 12) return false;

        if (FindMostGeodes(bps[0], 32) != 56) return false;
        if (FindMostGeodes(bps[1], 32) != 62) return false;
        */
        return true;
    }
}