#include "2019/d6_Orbits.h"

SOLUTION(2019, 6) {
    using Map = Constexpr::SmallMap<std::string, std::string>;
    using Cache = Constexpr::SmallMap<std::string, u32>;

    constexpr void BuildMap(const auto& lines, Map & outMap, Constexpr::SmallSet<std::string>& outSet) {
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, ")");
            std::string lhs = std::string(split[0]);
            std::string rhs = std::string(split[1]);
            outMap[rhs] = lhs;
            outSet.insert(lhs);
            outSet.insert(rhs);
        }
    }

    constexpr u32 CalculatePath(const std::string & str, const Map & map, Cache & cache) {
        if (cache.contains(str)) {
            return cache.at(str);
        }
        if (!map.contains(str)) {
            cache[str] = 0;
            return 0;
        }

        auto val = CalculatePath(map.at(str), map, cache) + 1;
        cache[str] = val;
        return val;
    }

    constexpr void BuildData(const auto& lines, Map& outMap, Cache& outCache) {
        Constexpr::SmallSet<std::string> all;
        BuildMap(lines, outMap, all);

        for (const auto& str : all) {
            CalculatePath(str, outMap, outCache);
        }
    }

    constexpr std::string FindCommonAncestor(const Map & map, const Cache & cache, const std::string lhs, const std::string rhs) {
        std::string a = lhs;
        std::string b = rhs;
        while (a != b) {
            if (cache.at(a) < cache.at(b)) {
                b = map.at(b);
            }
            else if (cache.at(b) < cache.at(a)) {
                a = map.at(a);
            }
            else {
                a = map.at(a);
                b = map.at(b);
            }
        }

        return a;
    }

    PART_ONE() {
        Map map;
        Cache cache;
        BuildData(Lines, map, cache);

        auto vals = cache.GetValues();
        return Constexpr::ToString(std::accumulate(vals.begin(), vals.end(), 0));
    }

    PART_TWO() {
        Map map;
        Cache cache;
        BuildData(Lines, map, cache);
        auto shared = FindCommonAncestor(map, cache, "YOU", "SAN");

        return Constexpr::ToString(cache.at("YOU") + cache.at("SAN") - (cache.at(shared) * 2) - 2);
    }

    TESTS() {
        std::vector<std::string> lines = {
            "COM)B",
            "B)C",
            "C)D",
            "D)E",
            "E)F",
            "B)G",
            "G)H",
            "D)I",
            "E)J",
            "J)K",
            "K)L",
            "K)YOU",
            "I)SAN"
        };
        Map map;
        Cache cache;
        BuildData(lines, map, cache);
        auto shared = FindCommonAncestor(map, cache, "YOU", "SAN");
        if (cache.at("YOU") + cache.at("SAN") - (cache.at(shared) * 2) - 2 != 4) return false;
        //if (PartTwo(lines) != "4") return false;

        return true;
    }
}