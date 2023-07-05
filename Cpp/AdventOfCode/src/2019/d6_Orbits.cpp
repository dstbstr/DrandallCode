#include "2019/d6_Orbits.h"

SOLUTION(2019, 6) {
    using Map = std::unordered_map<std::string, std::string>;
    using Cache = std::unordered_map<std::string, u32>;

    void BuildMap(const std::vector<std::string>&lines, Map & outMap, std::unordered_set<std::string>&outSet) {
        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, ")");
            std::string lhs = std::string(split[0]);
            std::string rhs = std::string(split[1]);
            outMap[rhs] = lhs;
            outSet.insert(lhs);
            outSet.insert(rhs);
        }
    }

    u32 CalculatePath(const std::string & str, const Map & map, Cache & cache) {
        if (cache.find(str) != cache.end()) {
            return cache[str];
        }
        if (map.find(str) == map.end()) {
            cache[str] = 0;
            return 0;
        }

        auto val = CalculatePath(map.at(str), map, cache) + 1;
        cache[str] = val;
        return val;
    }

    void BuildData(const std::vector<std::string>&lines, Map & outMap, Cache & outCache) {
        std::unordered_set<std::string> all;
        BuildMap(lines, outMap, all);

        for (const auto& str : all) {
            CalculatePath(str, outMap, outCache);
        }
    }

    auto Part1(const std::vector<std::string>&lines) {
        Map map;
        Cache cache;
        BuildData(lines, map, cache);

        u32 result = 0;
        for (const auto& [str, val] : cache) {
            result += val;
        }

        return result;
    }

    std::string FindCommonAncestor(const Map & map, const Cache & cache, const std::string lhs, const std::string rhs) {
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

    auto Part2(const std::vector<std::string>&lines) {
        Map map;
        Cache cache;
        BuildData(lines, map, cache);
        auto shared = FindCommonAncestor(map, cache, "YOU", "SAN");

        return cache.at("YOU") + cache.at("SAN") - (cache.at(shared) * 2) - 2;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
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

        if (Part2(lines) != 4) return false;
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