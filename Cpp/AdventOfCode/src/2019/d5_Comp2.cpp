#include "2019/d5_Comp2.h"
#include "2019/Comp.h"

SOLUTION(2019, 5) {
    constexpr s64 Solve(std::string_view line, s64 input) {
        auto code = ParseInstructions(line);
        Args args;
        args.Inputs.push_back(input);

        while (Apply(code, args));
        return args.Output;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Line, 1));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Line, 5));
    }

    TESTS() {
        return true;
    }
}