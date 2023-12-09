#include "2023/d8_Maps.h"
#include "Algorithms/FloydCycle.h"

SOLUTION(2023, 8) {

    using Map = Constexpr::BigMap<std::string, std::pair<std::string, std::string>>;
    constexpr Map CreateMap(const std::vector<std::string>&lines) {
        Constexpr::BigMap<std::string, std::pair<std::string, std::string>> map;
        for (const auto& line : lines) {
            auto s1 = Constexpr::Split(line, " = ");
            auto key = std::string(s1[0]);

            auto s2 = Constexpr::Split(s1[1], ", ");
            auto left = std::string(s2[0].substr(1));
            auto right = std::string(s2[1].substr(0, 3));

            map[key] = std::make_pair(left, right);
        }

        return map;
    }
    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);

        auto dirs = groups[0][0];
        auto map = CreateMap(groups[1]);

        std::string current = "AAA";
        u64 step = 0;
        while (current != "ZZZ") {
            auto dir = dirs[step++ % dirs.size()];
            if (dir == 'L') current = map[current].first;
            else current = map[current].second;
        }

        return Constexpr::ToString(step);
    }

    constexpr std::pair<std::string, size_t> Next2(std::pair<std::string, size_t> current, const std::string& dirs, const Map& map) {
        auto dir = dirs[current.second];
        auto next = current.first;
        if (dir == 'L') next = map.at(next).first;
        else next = map.at(next).second;

        return std::make_pair(next, (current.second + 1) % dirs.size());
    }
    constexpr void Next(std::string& pos, size_t& step, const std::string& dirs, const Map& map) {
        auto dir = dirs[step++];
        if (dir == 'L') pos = map.at(pos).first;
        else pos = map.at(pos).second;

        step %= dirs.size();
    }

    constexpr std::pair<size_t, size_t> FindCycle(const std::string& startingPoint, const std::string& dirs, const Map& map) {
        Constexpr::BigSet<std::pair<size_t, std::string>> seen;
        size_t cycleLength = 0;
        std::pair<size_t, std::string> pos = std::make_pair(0, startingPoint);
        while (!seen.contains(pos)) {
            cycleLength++;
            seen.insert(pos);
            Next(pos.second, pos.first, dirs, map);
        }

        size_t offset = 0;
        while (pos.second[2] != 'Z') {
            Next(pos.second, pos.first, dirs, map);
            offset++;
        }

        return std::make_pair(cycleLength, offset);
    }

    constexpr std::vector<size_t> FindEndPositions(const std::string& startPos, size_t cycleLength, size_t cycleStart, const std::string& dirs, const Map& map) {
        std::vector<size_t> result;
        auto pos = std::make_pair(startPos, 0ULL);

        for (auto i = 0; i < cycleStart; i++) {
            pos = Next2(pos, dirs, map);
        }
        for (auto i = 0; i < cycleLength; i++) {
            if (pos.first[2] == 'Z') result.push_back(i);
            pos = Next2(pos, dirs, map);
        }

        return result;
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);

        auto dirs = groups[0][0];
        auto map = CreateMap(groups[1]);

        std::vector<std::string> positions;
        for (auto pos : map.GetKeys()) {
            if (pos[2] == 'A') positions.push_back(pos);
        }

        //cycle is when the step and key is the same
        // cycle offset is how many steps after cycle to reach 'Z'
        //all keys are at end at the LCM of the cycles
        // 6 starting points

        
        std::vector<size_t> cycleLengths;
        for (const auto& pos : positions) {
            u32 cycleLength, cycleStart;
            auto start = std::make_pair(pos, 0ULL);
            FloydCycle::FindCycle(start, Next2, cycleLength, cycleStart, dirs, map);
            cycleLengths.push_back(cycleLength);
        }

        return Constexpr::ToString(Constexpr::FindLcm(cycleLengths));
    }

    TESTS() {
        std::vector<std::string> example = {
            "RL",
            "",
            "AAA = (BBB, CCC)",
            "BBB = (DDD, EEE)",
            "CCC = (ZZZ, GGG)",
            "DDD = (DDD, DDD)",
            "EEE = (EEE, EEE)",
            "GGG = (GGG, GGG)",
            "ZZZ = (ZZZ, ZZZ)"
        };

        if (PartOne(example) != "2") return false;

        std::vector<std::string> e2 = {
            "LR",
            "",
            "11A = (11B, XXX)",
            "11B = (XXX, 11Z)",
            "11Z = (11B, XXX)",
            "22A = (22B, XXX)",
            "22B = (22C, 22C)",
            "22C = (22Z, 22Z)",
            "22Z = (22B, 22B)",
            "XXX = (XXX, XXX)"
        };

        if (PartTwo(e2) != "6") return false;
        return true;
    }
}