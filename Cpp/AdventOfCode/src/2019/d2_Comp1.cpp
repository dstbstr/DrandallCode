#include "2019/d2_Comp1.h"
#include "2019/Comp.h"

SOLUTION(2019, 2) {

    PART_ONE() {
        auto instructions = ParseInstructions(lines[0]);
        instructions[1] = 12;
        instructions[2] = 2;

        Args args;
        while (Apply(instructions, args));

        return Constexpr::ToString(instructions[0]);
    }

    PART_TWO() {
        auto initialInstructions = ParseInstructions(lines[0]);
        Args initialArgs;
        for (u64 noun = 0; noun < 100; noun++) {
            for (u64 verb = 0; verb < 100; verb++) {
                auto instructions = initialInstructions;
                auto args = initialArgs;
                instructions[1] = noun;
                instructions[2] = verb;
                while (Apply(instructions, args));

                if (instructions[0] == 19690720) {
                    return Constexpr::ToString((noun * 100) + verb);
                }
            }
        }

        return 0ull;
    }

    TESTS() {
        std::vector<s64> inst = { 1,1,1,4,99,5,6,0,99 };
        Args args;
        while (Apply(inst, args));
        if (inst[0] != 30) return false;

        return true;
    }
}