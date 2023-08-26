#include "2020/d8_Halting.h"

SOLUTION(2020, 8) {
    struct State {
        s64 Accumulator{ 0 };
        s64 Ip{ 0 };
    };

    enum struct OpCode { Nop, Acc, Jmp };
    struct Inst {
        OpCode Op{ OpCode::Nop };
        s64 Argument{ 0 };
    };

    constexpr Inst ParseInst(std::string_view line) {
        auto split = Constexpr::Split(line, " ");
        s64 arg;
        Constexpr::ParseNumber(split[1], arg);
        Inst result;
        result.Argument = arg;

        if (split[0] == "nop") {
            result.Op = OpCode::Nop;
        }
        else if (split[0] == "acc") {
            result.Op = OpCode::Acc;
        }
        else if (split[0] == "jmp") {
            result.Op = OpCode::Jmp;
        }
        else {
            throw "Unknown op code" + std::string(split[0]);
        }

        return result;
    }

    constexpr void Apply(State & state, const Inst & inst) {
        switch (inst.Op) {
        case OpCode::Nop: break;
        case OpCode::Acc: state.Accumulator += inst.Argument; break;
        case OpCode::Jmp: state.Ip += inst.Argument; return;
        }
        state.Ip++;
    }

    constexpr bool Terminates(const std::vector<Inst>&instructions, State & state) {
        Constexpr::SmallSet<s64> seen;
        while (true) {
            if (seen.contains(state.Ip)) {
                return false;
            }
            if (state.Ip >= static_cast<s64>(instructions.size())) {
                return true;
            }
            seen.insert(state.Ip);
            Apply(state, instructions[state.Ip]);
        }
    }

    constexpr s64 SolvePartOne(const auto& lines) {
        const auto instructions = ParseLines(lines, ParseInst);

        State state;
        Terminates(instructions, state);
        return state.Accumulator;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    constexpr s64 SolvePartTwo(const auto& lines) {
        const auto instructions = ParseLines(lines, ParseInst);

        for (size_t i = 0; i < instructions.size(); i++) {
            if (instructions[i].Op == OpCode::Acc) continue;
            auto copy = instructions;
            if (instructions[i].Op == OpCode::Nop) {
                copy[i].Op = OpCode::Jmp;
            }
            else {
                copy[i].Op = OpCode::Nop;
            }
            State state;
            if (Terminates(copy, state)) {
                return state.Accumulator;
            }
        }

        throw "Not Found";
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(Lines));
    }

    TESTS() {
        std::vector<std::string> lines = {
           "nop +0",
           "acc +1",
           "jmp +4",
           "acc +3",
           "jmp -3",
           "acc -99",
           "acc +1",
           "jmp -4",
           "acc +6"
        };

        if (SolvePartOne(lines) != 5) return false;
        if (SolvePartTwo(lines) != 8) return false;
        return true;
    }
}