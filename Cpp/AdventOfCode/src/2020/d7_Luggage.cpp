#include "2020/d7_Luggage.h"
#include <queue>

SOLUTION(2020, 7) {
    //muted blue bags contain 1 vibrant lavender bag, 4 dotted silver bags, 2 dim indigo bags.
    //dull teal bags contain 2 bright tomato bags.
    //bright orange bags contain no other bags.
    using Map = Constexpr::SmallMap<std::string, Constexpr::SmallMap<std::string, size_t>>;

    constexpr Map BuildMap(const auto& lines) {
        Map result;
        for (const auto& line : lines) {
            auto split1 = Constexpr::Split(line, " contain ");
            auto split2 = Constexpr::Split(split1[0], " ");
            auto key = std::string(split2[0]) + " " + std::string(split2[1]);

            Constexpr::SmallMap<std::string, size_t> entry;
            if (std::isdigit(split1[1][0])) {
                auto split3 = Constexpr::Split(split1[1], ", ");
                for (auto bag : split3) {
                    auto split4 = Constexpr::Split(bag, " ");
                    size_t count;
                    Constexpr::ParseNumber(split4[0], count);
                    auto innerKey = std::string(split4[1]) + " " + std::string(split4[2]);
                    entry[innerKey] = count;
                }
                result[key] = entry;
            }
        }
        return result;
    }

    using BagCache = Constexpr::SmallMap<std::string, size_t>;

    size_t Recurse(const Map & map, BagCache & cache, const std::string & bag) {
        if (cache.contains(bag)) {
            return cache.at(bag);
        }
        if (!map.contains(bag)) {
            return 1;
        }

        size_t result = 1;
        for (const auto& [key, count] : map.at(bag)) {
            result += (Recurse(map, cache, key) * count);
        }
        cache[bag] = result;
        return result;
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto map = BuildMap(lines);
        Constexpr::SmallSet<std::string> seen;
        Constexpr::Queue<std::string> current;
        current.push("shiny gold");
        while (!current.is_empty()) {
            auto toFind = current.front();
            current.pop();

            for (const auto [key, value] : map) {
                if (value.contains(toFind)) {
                    if (!seen.contains(key)) {
                        current.push(key);
                        seen.insert(key);
                    }
                }
            }
        }

        return seen.size();
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto map = BuildMap(lines);
        BagCache cache;
        return Recurse(map, cache, "shiny gold") - 1;
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(Lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "light red bags contain 1 bright white bag, 2 muted yellow bags.",
            "dark orange bags contain 3 bright white bags, 4 muted yellow bags.",
            "bright white bags contain 1 shiny gold bag.",
            "muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.",
            "shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.",
            "dark olive bags contain 3 faded blue bags, 4 dotted black bags.",
            "vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.",
            "faded blue bags contain no other bags.",
            "dotted black bags contain no other bags."
        };

        if (SolvePartOne(lines) != 4) return false;
        if (SolvePartTwo(lines) != 32) return false;

        lines = {
            "shiny gold bags contain 2 dark red bags.",
            "dark red bags contain 2 dark orange bags.",
            "dark orange bags contain 2 dark yellow bags.",
            "dark yellow bags contain 2 dark green bags.",
            "dark green bags contain 2 dark blue bags.",
            "dark blue bags contain 2 dark violet bags.",
            "dark violet bags contain no other bags."
        };
        if (SolvePartTwo(lines) != 126) return false;

        return true;
    }
}