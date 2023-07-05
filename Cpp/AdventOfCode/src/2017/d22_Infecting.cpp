#include "2017/d22_Infecting.h"
#include "Facing.h"

SOLUTION(2017, 22) {
    std::unordered_set<Coord> ParseInput(const std::vector<std::string>&lines) {
        std::unordered_set<Coord> result;
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
                if (lines[row][col] == '#') {
                    result.insert({ static_cast<s32>(col), static_cast<s32>(row) });
                }
            }
        }

        return result;
    }

    auto Part1(const std::vector<std::string>&lines, u32 iterations) {
        auto infected = ParseInput(lines);
        u32 count = 0;
        Facing facing = Up;
        Coord currentPos;
        currentPos.Y = static_cast<s32>(lines.size() / 2);
        currentPos.X = static_cast<s32>(lines[0].size() / 2);

        for (u32 i = 0; i < iterations; i++) {
            if (infected.find(currentPos) != infected.end()) {
                facing = TurnRight(facing);
                infected.erase(currentPos);
            }
            else {
                facing = TurnLeft(facing);
                infected.insert(currentPos);
                count++;
            }

            Move(currentPos, facing);
        }

        return count;
    }

    auto Part2(const std::vector<std::string>&lines, u32 iterations) {
        auto infected = ParseInput(lines);
        std::unordered_set<Coord> weakened{};
        std::unordered_set<Coord> flagged{};
        Facing facing = Up;
        Coord currentPos;
        currentPos.Y = static_cast<s32>(lines.size() / 2);
        currentPos.X = static_cast<s32>(lines[0].size() / 2);

        u32 count = 0;

        for (u32 i = 0; i < iterations; i++) {
            if (infected.find(currentPos) != infected.end()) {
                facing = TurnRight(facing);
                infected.erase(currentPos);
                flagged.insert(currentPos);
            }
            else if (weakened.find(currentPos) != weakened.end()) {
                weakened.erase(currentPos);
                infected.insert(currentPos);
                count++;
            }
            else if (flagged.find(currentPos) != flagged.end()) {
                facing = TurnAround(facing);
                flagged.erase(currentPos);
            }
            else {
                facing = TurnLeft(facing);
                weakened.insert(currentPos);
            }

            Move(currentPos, facing);
        }

        return count;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 10000));
        return Constexpr::ToString(Part2(lines, 10'000'000));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "..#",
            "#..",
            "..."
        };

        if (Part1(lines, 70) != 41) return false;
        if (Part1(lines, 10000) != 5587) return false;
        if (Part2(lines, 100) != 26) return false;
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