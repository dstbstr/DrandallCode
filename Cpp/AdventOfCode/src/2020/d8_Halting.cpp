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

    constexpr Inst ParseInst(const std::string & line) {
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

    constexpr std::vector<Inst> ParseInstructions(const std::vector<std::string>&lines) {
        std::vector<Inst> result;
        for (const auto& line : lines) {
            result.push_back(ParseInst(line));
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

    bool Terminates(const std::vector<Inst>&instructions, State & state) {
        std::unordered_set<s64> seen;
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

    auto Part1(const std::vector<std::string>&lines) {
        const auto instructions = ParseInstructions(lines);

        State state;
        Terminates(instructions, state);
        return state.Accumulator;
    }

    auto Part2(const std::vector<std::string>&lines) {
        const auto instructions = ParseInstructions(lines);

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

        return 0ll;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
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

        if (Part1(lines) != 5) return false;
        if (Part2(lines) != 8) return false;
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