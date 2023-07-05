#include "2017/d5_Trampoline.h"

SOLUTION(2017, 5) {

    constexpr void Jump(u32 & instruction, s32 & distance) {
        instruction += distance;
        distance++;
    }

    constexpr void ShortJump(u32 & instruction, s32 & distance) {
        instruction += distance;
        if (distance > 2) {
            distance--;
        }
        else {
            distance++;
        }
    }

    constexpr u32 StepsToExit(const std::vector<std::string> lines, auto jumpFunc) {
        auto jumps = ParseLinesAsNumbers<s32>(lines);

        u32 steps = 0;
        u32 instruction = 0;
        while (instruction < jumps.size()) {
            jumpFunc(instruction, jumps[instruction]);
            steps++;
        }

        return steps;
    }

    PART_ONE() {
        return Constexpr::ToString(StepsToExit(lines, Jump));
    }

    PART_TWO() {
        return Constexpr::ToString(StepsToExit(lines, ShortJump));
    }

    TESTS() {
        static_assert(StepsToExit({ "0", "3", "0", "1", "-3" }, Jump) == 5);
        static_assert(StepsToExit({ "0", "3", "0", "1", "-3" }, ShortJump) == 10);

        return true;
    }
}