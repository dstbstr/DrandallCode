#include "2017/d7_Circus.h"

SOLUTION(2017, 7) {
    struct Node {
        std::string Name = "";
        u32 Weight = 0;
        std::vector<std::string> Carrying{};
    };

    using Map = Constexpr::SmallMap<std::string, Node>;

    constexpr Node ParseNode(std::string_view line) {
        auto split = Constexpr::Split(line, " ");
        Node node;
        node.Name = split[0];
        Constexpr::ParseNumber(split[1].substr(1, split[1].size() - 2), node.Weight);

        for (auto i = 3; i < split.size(); i++) {
            if (split[i].back() == ',') {
                node.Carrying.push_back(std::string(split[i].substr(0, split[i].size() - 1)));
            }
            else {
                node.Carrying.push_back(std::string(split[i]));
            }
        }
        return node;
    }

    constexpr u32 GetWeight(const Map & map, const std::string name) {
        u32 weight = map.at(name).Weight;
        for (auto child : map.at(name).Carrying) {
            weight += GetWeight(map, child);
        }

        return weight;
    }

    constexpr bool AllMatch(const std::vector<u32>&weights, size_t & outOddIndex) {
        auto copy = weights;
        std::sort(copy.begin(), copy.end());

        if (copy[0] == copy.back()) {
            return true;
        }
        u32 oddValue;
        if (copy[0] == copy[1]) {
            oddValue = copy.back();
        }
        else {
            oddValue = copy[0];
        }
        for (size_t i = 0; i < weights.size(); i++) {
            if (weights[i] == oddValue) {
                outOddIndex = i;
                return false;
            }
        }

        return false;
    }

    constexpr Map ParseMap(const auto& lines) {
        Map result;
        for (const auto& line : lines) {
            auto node = ParseNode(line);
            result[node.Name] = node;
        }

        return result;
    }

    constexpr std::string FindRoot(const Map & map) {
        auto carriedBy = Constexpr::SmallMap<std::string, std::string>{};
        for (const auto& [name, node] : map) {
            for (const auto& carry : node.Carrying) {
                carriedBy[carry] = name;
            }
        }

        for (const auto& [name, node] : map) {
            if (!carriedBy.contains(name)) {
                return name;
            }
        }

        throw std::logic_error("Not found");
    }

    PART_ONE() {
        auto map = ParseMap(lines);
        return FindRoot(map);
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto map = ParseMap(lines);
        auto tower = FindRoot(map);
        s32 delta = 0;

        while (true) {
            std::vector<u32> weights;
            for (const auto& child : map.at(tower).Carrying) {
                weights.push_back(GetWeight(map, child));
            }
            size_t oddIndex;
            if (AllMatch(weights, oddIndex)) {
                return map.at(tower).Weight + delta;
            }
            else {
                if (oddIndex == 0) {
                    delta = weights[1] - weights[oddIndex];
                }
                else {
                    delta = weights[0] - weights[oddIndex];
                }
                tower = map.at(tower).Carrying[oddIndex];
            }
        }

        throw std::logic_error("Not Found");
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    constexpr bool TestAllMatch(const std::vector<u32>& weights, bool shouldMatch, size_t expectedOddIndex) {
        size_t actualOddIndex;
        auto isMatch = AllMatch(weights, actualOddIndex);
        if (shouldMatch && isMatch) return true;
        return actualOddIndex == expectedOddIndex;
    }

    TESTS() {
        static_assert(ParseNode("ab (42)").Name == "ab");
        static_assert(ParseNode("ab (42)").Weight == 42);
        static_assert(ParseNode("ab (42) -> bc, cd").Carrying.size() == 2);
        static_assert(ParseNode("ab (42) -> bc, cd").Carrying[0] == "bc");
        
        static_assert(TestAllMatch({ 1, 1, 1 }, true, 0));
        static_assert(TestAllMatch({ 0, 1, 1 }, false, 0));
        static_assert(TestAllMatch({ 1, 0, 1 }, false, 1));
        static_assert(TestAllMatch({ 1, 1, 0 }, false, 2));

        std::vector<std::string> lines = {
            "pbga (66)",
            "xhth (57)",
            "ebii (61)",
            "havc (66)",
            "ktlj (57)",
            "fwft (72) -> ktlj, cntj, xhth",
            "qoyq (66)",
            "padx (45) -> pbga, havc, qoyq",
            "tknk (41) -> ugml, padx, fwft",
            "jptl (61)",
            "ugml (68) -> gyxo, ebii, jptl",
            "gyxo (61)",
            "cntj (57)"
        };

        if (FindRoot(ParseMap(lines)) != "tknk") return false;
        if(SolvePartTwo(lines) != 60) return false;

        return true;
    }
}