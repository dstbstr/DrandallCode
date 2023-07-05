#include "2019/d9_Comp4.h"

SOLUTION(2019, 9) {
    struct Args {
        std::vector<s64> Inputs;
        s64 Output = 0;
        s64 Base = 0;
        size_t CurrentInput = 0;
        size_t Ip = 0;
    };

    enum OpCode { Add = 1, Mul = 2, Set = 3, Print = 4, Jnz = 5, Jz = 6, Lt = 7, Eq = 8, MoveBase = 9, Halt = 99 };
    enum ParamMode { Position = 0, Immediate = 1, Relative = 2 };

    constexpr u8 ArgCount(OpCode code) {
        switch (code) {
        case Add: return 3;
        case Mul: return 3;
        case Set: return 1;
        case Print: return 1;
        case Jnz: return 2;
        case Jz: return 2;
        case Lt: return 3;
        case Eq: return 3;
        case MoveBase: return 1;
        case Halt: return 0;
        }

        return 0;
    }

    constexpr void ParseInstruction(s64 value, OpCode & opCode, std::vector<ParamMode>&modes) {
        opCode = static_cast<OpCode>(value % 100);
        value /= 100;
        while (value > 0) {
            modes.push_back(static_cast<ParamMode>(value % 10));
            value /= 10;
        }
    }

    constexpr s64 Read(const std::vector<s64>&instructions, const std::pair<s64, ParamMode>&param, size_t base) {
        const auto& [value, mode] = param;

        if (mode == ParamMode::Immediate) {
            return value;
        }

        size_t targetIndex = value;
        if (mode == ParamMode::Relative) {
            targetIndex += base;
        }

        if (instructions.size() <= targetIndex) {
            return 0;
        }
        else {
            return instructions[targetIndex];
        }
    }

    constexpr s64& GetRegister(std::vector<s64>&instructions, const std::pair<s64, ParamMode>&param, size_t base) {
        const auto& [value, mode] = param;
        size_t targetIndex = value;
        if (mode == ParamMode::Relative) {
            targetIndex += base;
        }

        while (instructions.size() <= targetIndex) {
            instructions.push_back(0);
        }
        return instructions[targetIndex];
    }

    constexpr bool Apply(std::vector<s64>&instructions, Args & args) {
        OpCode op;
        std::vector<ParamMode> modes;
        ParseInstruction(instructions[args.Ip], op, modes);
        std::vector<std::pair<s64, ParamMode>> parameters;
        for (auto i = 0; i < ArgCount(op); i++) {
            auto pIndex = args.Ip + i + 1;
            auto val = instructions[pIndex];
            auto mode = modes.size() <= i ? ParamMode::Position : modes[i];
            parameters.push_back(std::make_pair(val, mode));
        }

        switch (op)
        {
        case Halt: {
            return false;
        }
        case Add: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& c = GetRegister(instructions, parameters[2], args.Base);
            c = a + b;
            break;
        }
        case Mul: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& c = GetRegister(instructions, parameters[2], args.Base);
            c = a * b;
            break;
        }
        case Set: {
            auto& reg = GetRegister(instructions, parameters[0], args.Base);
            reg = args.Inputs[args.CurrentInput++];
            break;
        }
        case Print: {
            args.Output = Read(instructions, parameters[0], args.Base);
            break;
        }
        case Jnz: {
            auto a = Read(instructions, parameters[0], args.Base);
            if (a != 0) {
                args.Ip = Read(instructions, parameters[1], args.Base);
                return true;
            }
            break;
        }
        case Jz: {
            auto a = Read(instructions, parameters[0], args.Base);
            if (a == 0) {
                args.Ip = Read(instructions, parameters[1], args.Base);
                return true;
            }
            break;
        }
        case Lt: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& reg = GetRegister(instructions, parameters[2], args.Base);
            reg = a < b ? 1 : 0;
            break;
        }
        case Eq: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& reg = GetRegister(instructions, parameters[2], args.Base);
            reg = a == b ? 1 : 0;
            break;
        }
        case MoveBase: {
            auto a = Read(instructions, parameters[0], args.Base);
            args.Base += a;
            break;
        }
        }

        args.Ip += parameters.size() + 1;
        return true;
    }


    bool ApplyDebug(std::vector<s64>&instructions, Args & args) {
        OpCode op;
        std::vector<ParamMode> modes;
        ParseInstruction(instructions[args.Ip], op, modes);
        std::vector<std::pair<s64, ParamMode>> parameters;
        for (auto i = 0; i < ArgCount(op); i++) {
            auto pIndex = args.Ip + i + 1;
            auto val = instructions[pIndex];
            auto mode = modes.size() <= i ? ParamMode::Position : modes[i];
            parameters.push_back(std::make_pair(val, mode));
        }

        std::cout << "IP: " << args.Ip << ": ";
        switch (op)
        {
        case Halt: {
            std::cout << "Halt\n";
            return false;
        }
        case Add: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& c = GetRegister(instructions, parameters[2], args.Base);
            c = a + b;
            std::cout << "Add: Instruction[" << parameters[2].first << "] = " << a << " + " << b << " (" << c << ")\n";
            break;
        }
        case Mul: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& c = GetRegister(instructions, parameters[2], args.Base);
            c = a * b;
            std::cout << "Mul: Instruction[" << parameters[2].first << "] = " << a << " * " << b << " (" << c << ")\n";
            break;
        }
        case Set: {
            auto& reg = GetRegister(instructions, parameters[0], args.Base);
            reg = args.Inputs[args.CurrentInput++];
            std::cout << "Set: Instruction[" << parameters[0].first << "] = " << reg << "\n";
            break;
        }
        case Print: {
            args.Output = Read(instructions, parameters[0], args.Base);
            std::cout << "Print: Output = " << args.Output << "\n";
            break;
        }
        case Jnz: {
            auto a = Read(instructions, parameters[0], args.Base);
            std::cout << "Jnz: " << a;

            if (a != 0) {
                args.Ip = Read(instructions, parameters[1], args.Base);
                std::cout << " Ip = " << args.Ip << "\n";
                return true;
            }
            std::cout << " Did not jump\n";
            break;
        }
        case Jz: {
            auto a = Read(instructions, parameters[0], args.Base);
            std::cout << "Jz: " << a;
            if (a == 0) {
                args.Ip = Read(instructions, parameters[1], args.Base);
                std::cout << " Ip = " << args.Ip << "\n";
                return true;
            }
            std::cout << " Did not jump\n";
            break;
        }
        case Lt: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& reg = GetRegister(instructions, parameters[2], args.Base);
            reg = a < b ? 1 : 0;
            std::cout << "Lt: Instruction[" << parameters[2].first << "] = " << a << " < " << b << " (" << reg << ")\n";
            break;
        }
        case Eq: {
            auto a = Read(instructions, parameters[0], args.Base);
            auto b = Read(instructions, parameters[1], args.Base);
            auto& reg = GetRegister(instructions, parameters[2], args.Base);
            reg = a == b ? 1 : 0;
            std::cout << "Eq: Instructions[" << parameters[2].first << "] = " << a << " == " << b << " (" << reg << ")\n";
            break;
        }
        case MoveBase: {
            auto a = Read(instructions, parameters[0], args.Base);
            std::cout << "MoveBase: " << args.Base << " + " << a;
            args.Base += a;
            std::cout << " (" << args.Base << ")\n";
            break;
        }
        }

        args.Ip += parameters.size() + 1;
        return true;
    }


    constexpr bool TestApply(std::vector<s64> start, std::vector<s64> expectedOutput) {
        Args args;
        while (Apply(start, args));

        return start == expectedOutput;
    }

    constexpr bool TestApply(std::vector<s64> start, s64 input, s64 expectedOutput) {
        Args args;
        args.Inputs.push_back(input);
        while (Apply(start, args));

        return args.Output == expectedOutput;
    }

    static_assert(TestApply({ 1, 0, 0, 0, 99 }, { 2,0,0,0,99 }));
    static_assert(TestApply({ 2,3,0,3,99 }, { 2,3,0,6,99 }));
    static_assert(TestApply({ 2,4,4,5,99,0 }, { 2,4,4,5,99,9801 }));
    static_assert(TestApply({ 1002,4,3,4,33 }, { 1002,4,3,4,99 }));
    static_assert(TestApply({ 1101,100,-1,4,0 }, { 1101,100,-1,4,99 }));

    static_assert(TestApply({ 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9 }, 0, 0)); // == 0
    static_assert(TestApply({ 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9 }, 24, 1)); // == 0
    static_assert(TestApply({ 3,9,8,9,10,9,4,9,99,-1,8 }, 8, 1)); // == 8
    static_assert(TestApply({ 3,9,8,9,10,9,4,9,99,-1,8 }, 100, 0)); // == 8
    static_assert(TestApply({ 3,9,7,9,10,9,4,9,99,-1,8 }, 5, 1)); // less than 8
    static_assert(TestApply({ 3,9,7,9,10,9,4,9,99,-1,8 }, 10, 0)); // less than 8

    constexpr std::vector<s64> ParseLine(const std::string & line) {
        std::vector<s64> result{};
        auto split = Constexpr::Split(line, ",");
        for (const auto elem : split) {
            s64 val;
            Constexpr::ParseNumber(elem, val);
            result.push_back(val);
        }

        return result;
    }

    s64 PartOne(const std::string & line) {
        auto instructions = ParseLine(line);
        Args args;
        args.Inputs.push_back(1);

        while (Apply(instructions, args));

        return args.Output;
    }

    auto Part2(const std::string & line) {
        auto instructions = ParseLine(line);
        Args args;
        args.Inputs.push_back(2);

        while (Apply(instructions, args));

        return args.Output;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        std::string line = "104,1125899906842624,99";
        auto instructions = ParseLine(line);
        Args args;
        Apply(instructions, args);
        if (args.Output != 1125899906842624) return false;

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