#include "2019/d9_Comp4.h"
#include "2019/Comp.h"

SOLUTION(2019, 9) {
    constexpr s64 Solve(const std::string& line, s64 input) {
        auto instructions = ParseInstructions(line);
        Args args;
        args.Inputs.push_back(input);

        while (Apply(instructions, args));

        return args.Output;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines[0], 1));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines[0], 2));
    }

    TESTS() {
        return true;
    }
}