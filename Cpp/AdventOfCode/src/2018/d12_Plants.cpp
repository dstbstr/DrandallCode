#include "2018/d12_Plants.h"

SOLUTION(2018, 12) {
    std::unordered_map<std::string, char> ParseRules(const std::vector<std::string>&lines) {
        std::unordered_map<std::string, char> rules;
        for (auto i = 2; i < lines.size(); i++) {
            auto split = StrUtil::Split(lines[i], " => ");
            rules[std::string(split[0])] = split[1][0];
        }

        return rules;
    }

    std::string NextGeneration(const std::string & pots, const std::unordered_map<std::string, char>&rules) {
        std::string running = pots.substr(0, 5);
        auto next = pots + ".";
        for (auto i = 4; i < pots.size() - 1; i++) {
            next[i - 2] = rules.at(running);
            running = running.substr(1) + pots[i + 1];
        }

        return next;
    }

    constexpr auto CountPots(const std::string & pots, s32 prefixSize) {
        s32 result = 0;
        for (s32 i = 0; i < pots.size(); i++) {
            result += (pots[i] == '#') * (i - prefixSize);
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines, u32 generations) {
        static const auto prefix = "......"s;
        static const s32 prefixSize = static_cast<s32>(prefix.size());
        auto current = prefix + lines[0].substr(15) + "...";
        auto rules = ParseRules(lines);

        for (u32 generation = 0; generation < generations; generation++) {
            current = NextGeneration(current, rules);
        }

        return CountPots(current, prefixSize);
    }

    auto Part2(const std::vector<std::string>&lines) {
        static const auto prefix = "......"s;
        static const auto prefixSize = static_cast<s32>(prefix.size());

        auto pots = prefix + lines[0].substr(15) + "...";
        auto rules = ParseRules(lines);

        s32 currentCount = 0;
        s32 previousCount = 0;
        s32 previousDelta = 1;
        s32 currentDelta = 0;
        u32 generation = 0;
        while (currentDelta != previousDelta) {
            generation++;
            previousDelta = currentDelta;
            previousCount = currentCount;

            currentCount = CountPots(pots, prefixSize);
            currentDelta = currentCount - previousCount;

            pots = NextGeneration(pots, rules);
        }

        u64 result = 50'000'000'000 - (generation - 1);
        result *= 21;
        result += currentCount;
        return result;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 20));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "initial state: #..#.#..##......###...###",
            "",
            "..... => .",
            "....# => .",
            "...#. => .",
            "...## => #",
            "..#.. => #",
            "..#.# => .",
            "..##. => .",
            "..### => .",
            ".#... => #",
            ".#..# => .",
            ".#.#. => #",
            ".#.## => #",
            ".##.. => #",
            ".##.# => .",
            ".###. => .",
            ".#### => #",
            "#.... => .",
            "#...# => .",
            "#..#. => .",
            "#..## => .",
            "#.#.. => .",
            "#.#.# => #",
            "#.##. => .",
            "#.### => #",
            "##... => .",
            "##..# => .",
            "##.#. => #",
            "##.## => #",
            "###.. => #",
            "###.# => #",
            "####. => #",
            "##### => ."
        };

        if (Part1(lines, 20) != 325) return false;
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