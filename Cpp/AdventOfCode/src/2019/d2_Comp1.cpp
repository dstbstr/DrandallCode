#include "2019/d2_Comp1.h"
#include "2019/Comp.h"

SOLUTION(2019, 2) {
    /*
    enum OpCode { Add = 1, Mul = 2, Halt = 99 };
    constexpr u8 ArgCount(OpCode code) {
        switch (code) {
        case Add: return 3;
        case Mul: return 3;
        case Halt: return 0;
        }

        return 0;
    }

    constexpr bool Apply(std::vector<u64>&instructions, size_t & index) {
        auto op = static_cast<OpCode>(instructions[index]);
        std::vector<size_t> indexes;
        std::vector<u64> parameters;
        for (auto i = 0; i < ArgCount(op); i++) {
            auto pIndex = index + i + 1;
            indexes.push_back(instructions[pIndex]);
            parameters.push_back(instructions[indexes[i]]);
        }
        switch (op)
        {
        case Halt: return false;
        case Add:
            instructions[indexes[2]] = parameters[0] + parameters[1];
            break;
        case Mul:
            instructions[indexes[2]] = parameters[0] * parameters[1];
        }

        index += parameters.size() + 1;
        return true;
    }

    constexpr bool TestApply(std::vector<u64> input, size_t index, std::vector<u64> expectedOutput) {
        Apply(input, index);
        return input == expectedOutput;
    }

    static_assert(TestApply({ 1,0,0,0,99 }, 0, { 2,0,0,0,99 }));
    static_assert(TestApply({ 2,3,0,3,99 }, 0, { 2,3,0,6,99 }));
    static_assert(TestApply({ 2,4,4,5,99,0 }, 0, { 2,4,4,5,99,9801 }));

    constexpr std::vector<u64> ParseLine(const std::string & line) {
        std::vector<u64> result;
        auto split = Constexpr::Split(line, ",");
        for (const auto& e : split) {
            u64 val;
            Constexpr::ParseNumber(e, val);
            result.push_back(val);
        }

        return result;
    }

    auto Part1(const std::string & line) {
        size_t index = 0;
        auto instructions = ParseLine(line);
        instructions[1] = 12;
        instructions[2] = 2;
        while (Apply(instructions, index));

        return instructions[0];
    }

    auto Part2(const std::string & line, u64 target) {
        auto initial = ParseLine(line);

        for (u64 noun = 0; noun < 100; noun++) {
            for (u64 verb = 0; verb < 100; verb++) {
                auto instructions = initial;
                size_t index = 0;
                instructions[1] = noun;
                instructions[2] = verb;
                while (Apply(instructions, index));

                if (instructions[0] == target) {
                    return (noun * 100) + verb;
                }
            }
        }

        return 0ull;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0], 19690720));
    }

    bool RunTests() {
        std::vector<u64> inst = { 1,1,1,4,99,5,6,0,99 };
        size_t index = 0;
        while (Apply(inst, index));
        if (inst[0] != 30) return false;

        return true;
    }
    */

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