#include "2019/d3_Wires.h"
#include "Facing.h"

SOLUTION(2019, 3) {
    using Seen = std::unordered_set<Coord>;
    using StepMap = std::unordered_map<Coord, u32>;

    void ApplyMove(Coord & pos, Facing dir, u32 steps, Seen & seen) {
        seen.insert(pos);
        for (u32 i = 0; i < steps; i++) {
            Move(pos, dir);
            seen.insert(pos);
        }
    }

    static u32 currentStep = 0; //gross?  You betcha

    void ApplyMove(Coord & pos, Facing dir, u32 steps, StepMap & stepMap) {
        if (!stepMap.contains(pos)) {
            stepMap[pos] = currentStep;
        }
        for (u32 i = 0; i < steps; i++) {
            Move(pos, dir);
            currentStep++;
            if (!stepMap.contains(pos)) {
                stepMap[pos] = currentStep;
            }
        }
    }

    constexpr std::vector<std::pair<Facing, u32>> ParseWire(const std::string & line) {
        std::vector<std::pair<Facing, u32>> result;

        auto split = Constexpr::Split(line, ",");
        for (const auto& v : split) {
            Facing dir = Up;

            switch (v[0]) {
            case 'U': dir = Up; break;
            case 'D': dir = Down; break;
            case 'L': dir = Left; break;
            case 'R': dir = Right; break;
            }

            u32 amount;
            auto numStr = v.substr(1);
            Constexpr::ParseNumber(numStr, amount);
            result.push_back(std::make_pair(dir, amount));
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines) {
        const Coord origin = { 0, 0 };
        auto w1Path = ParseWire(lines[0]);
        auto w2Path = ParseWire(lines[1]);
        Seen seen1;
        Coord pos1 = origin;

        for (const auto& [dir, steps] : w1Path) {
            ApplyMove(pos1, dir, steps, seen1);
        }

        Seen seen2;
        Coord pos2 = origin;
        for (const auto& [dir, steps] : w2Path) {
            ApplyMove(pos2, dir, steps, seen2);
        }

        size_t best = 999999;
        for (const auto& coord : seen1) {
            if (coord == origin) continue;
            if (seen2.find(coord) != seen2.end()) {
                best = std::min(best, MDistance(coord, origin));
            }
        }

        return best;
    }

    auto Part2(const std::vector<std::string>&lines) {
        const Coord origin = { 0, 0 };
        auto w1Path = ParseWire(lines[0]);
        auto w2Path = ParseWire(lines[1]);
        StepMap stepMap1;
        Coord pos1 = origin;

        for (const auto& [dir, steps] : w1Path) {
            ApplyMove(pos1, dir, steps, stepMap1);
        }

        currentStep = 0; //other half of the gross hack

        StepMap stepMap2;
        Coord pos2 = origin;
        for (const auto& [dir, steps] : w2Path) {
            ApplyMove(pos2, dir, steps, stepMap2);
        }

        currentStep = 0; //third half of the gross hack
        u32 best = 999999;
        for (const auto& [pos, steps] : stepMap1) {
            if (pos == origin) continue;
            if (stepMap2.contains(pos)) {
                best = std::min(best, steps + stepMap2.at(pos));
            }
        }

        return best;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "R75,D30,R83,U83,L12,D49,R71,U7,L72",
            "U62,R66,U55,R34,D71,R55,D58,R83"
        };

        if (Part1(lines) != 159) return false;
        if (Part2(lines) != 610) return false;

        lines = {
            "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
            "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"
        };

        if (Part1(lines) != 135) return false;
        if (Part2(lines) != 410) return false;
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