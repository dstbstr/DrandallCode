#include "2018/d12_Plants.h"

SOLUTION(2018, 12) {
    using Rules = Constexpr::SmallMap<std::string, char>;
    static const std::string Prefix = "......"s;
    static const size_t PrefixSize = Prefix.size();

    constexpr Rules ParseRules(const std::vector<std::string>&lines) {
        Rules rules;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " => ");
            rules[std::string(s[0])] = s[1][0];
        }

        return rules;
    }

    constexpr std::string NextGeneration(const std::string & pots, const Rules& rules) {
        std::string running = pots.substr(0, 5);
        auto next = pots + ".";
        for (auto i = 4; i < pots.size() - 1; i++) {
            next[i - 2] = rules.at(running);
            running = running.substr(1) + pots[i + 1];
        }

        return next;
    }

    constexpr auto CountPots(const std::string & pots) {
        size_t result = 0;
        for (size_t i = 0; i < pots.size(); i++) {
            result += (pots[i] == '#') * (i - PrefixSize);
        }

        return result;
    }

    constexpr std::string GetInitialState(const std::vector<std::string>& lines) {
        auto s = Constexpr::Split(lines[0], " ");
        return std::string(s[2]);
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);

        auto pots = Prefix + GetInitialState(groups[0]) + "...";
        auto rules = ParseRules(groups[1]);

        for (u32 generation = 0; generation < 20; generation++) {
            pots = NextGeneration(pots, rules);
        }

        return Constexpr::ToString(CountPots(pots));
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);

        auto pots = Prefix + GetInitialState(groups[0]) + "...";
        auto rules = ParseRules(groups[1]);

        size_t currentCount = 0;
        size_t previousCount = 0;
        size_t previousDelta = 1;
        size_t currentDelta = 0;
        size_t generation = 0;
        while (currentDelta != previousDelta) {
            generation++;
            previousDelta = currentDelta;
            previousCount = currentCount;

            currentCount = CountPots(pots);
            currentDelta = currentCount - previousCount;

            pots = NextGeneration(pots, rules);
        }

        size_t result = 50'000'000'000 - (generation - 1);
        result *= 21;
        result += currentCount;
        return Constexpr::ToString(result);
    }

    TESTS() {
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

        if (PartOne(lines) != "325") return false;
        return true;
    }
}