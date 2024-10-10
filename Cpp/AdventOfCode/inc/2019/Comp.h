#pragma once

#include "Common.h"

constexpr s64 Unset = -9919;

struct Args {
    std::vector<s64> Inputs;
    s64 Output = Unset;
    s64 Base = 0;
    size_t CurrentInput = 0;
    size_t Ip = 0;
};

enum OpCode { Add = 1, Mul = 2, Set = 3, Print = 4, Jnz = 5, Jz = 6, Lt = 7, Eq = 8, MoveBase = 9, Halt = 99 };
enum ParamMode { Position = 0, Immediate = 1, Relative = 2 };

namespace detail {
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

    constexpr void ParseInstruction(s64 value, OpCode& opCode, std::vector<ParamMode>& modes) {
        opCode = static_cast<OpCode>(value % 100);
        value /= 100;
        while (value > 0) {
            modes.push_back(static_cast<ParamMode>(value % 10));
            value /= 10;
        }
    }

    constexpr s64 Read(const std::vector<s64>& instructions, const std::pair<s64, ParamMode>& param, size_t base) {
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

    constexpr s64& GetRegister(std::vector<s64>& instructions, const std::pair<s64, ParamMode>& param, size_t base) {
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
}

constexpr bool Apply(std::vector<s64>& instructions, Args& args) {
    OpCode op;
    std::vector<ParamMode> modes;
    detail::ParseInstruction(instructions[args.Ip], op, modes);
    std::vector<std::pair<s64, ParamMode>> parameters;
    for (auto i = 0; i < detail::ArgCount(op); i++) {
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
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& c = detail::GetRegister(instructions, parameters[2], args.Base);
        c = a + b;
        break;
    }
    case Mul: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& c = detail::GetRegister(instructions, parameters[2], args.Base);
        c = a * b;
        break;
    }
    case Set: {
        auto& reg = detail::GetRegister(instructions, parameters[0], args.Base);
        reg = args.Inputs[args.CurrentInput++];
        break;
    }
    case Print: {
        args.Output = detail::Read(instructions, parameters[0], args.Base);
        break;
    }
    case Jnz: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        if (a != 0) {
            args.Ip = detail::Read(instructions, parameters[1], args.Base);
            return true;
        }
        break;
    }
    case Jz: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        if (a == 0) {
            args.Ip = detail::Read(instructions, parameters[1], args.Base);
            return true;
        }
        break;
    }
    case Lt: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& reg = detail::GetRegister(instructions, parameters[2], args.Base);
        reg = a < b ? 1 : 0;
        break;
    }
    case Eq: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& reg = detail::GetRegister(instructions, parameters[2], args.Base);
        reg = a == b ? 1 : 0;
        break;
    }
    case MoveBase: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        args.Base += a;
        break;
    }
    }

    args.Ip += parameters.size() + 1;
    return true;
}


inline bool ApplyDebug(std::vector<s64>& instructions, Args& args) {
    OpCode op;
    std::vector<ParamMode> modes;
    detail::ParseInstruction(instructions[args.Ip], op, modes);
    std::vector<std::pair<s64, ParamMode>> parameters;
    for (auto i = 0; i < detail::ArgCount(op); i++) {
        auto pIndex = args.Ip + i + 1;
        auto val = instructions[pIndex];
        auto mode = modes.size() <= i ? ParamMode::Position : modes[i];
        parameters.push_back(std::make_pair(val, mode));
    }

	std::string logMessage = "IP: " + std::to_string(args.Ip) + ": ";
    switch (op)
    {
    case Halt: {
        logMessage += "Halt";
        GET_LOGS().emplace_back(logMessage);
        return false;
    }
    case Add: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& c = detail::GetRegister(instructions, parameters[2], args.Base);
        c = a + b;
		logMessage += "Add: Instruction[" + std::to_string(parameters[2].first) + "] = " + std::to_string(a) + " + " + std::to_string(b) + " (" + std::to_string(c) + ")";
        break;
    }
    case Mul: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& c = detail::GetRegister(instructions, parameters[2], args.Base);
        c = a * b;
		logMessage += "Mul: Instruction[" + std::to_string(parameters[2].first) + "] = " + std::to_string(a) + " * " + std::to_string(b) + " (" + std::to_string(c) + ")";
        break;
    }
    case Set: {
        auto& reg = detail::GetRegister(instructions, parameters[0], args.Base);
        reg = args.Inputs[args.CurrentInput++];
		logMessage += "Set: Instruction[" + std::to_string(parameters[0].first) + "] = " + std::to_string(reg);
        break;
    }
    case Print: {
        args.Output = detail::Read(instructions, parameters[0], args.Base);
		logMessage += "Print: Output = " + std::to_string(args.Output);
        break;
    }
    case Jnz: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
		logMessage += "Jnz: " + std::to_string(a);

        if (a != 0) {
            args.Ip = detail::Read(instructions, parameters[1], args.Base);
			logMessage += " Ip = " + std::to_string(args.Ip);
			GET_LOGS().emplace_back(logMessage);
            return true;
        }
		logMessage += " Did not jump";
        break;
    }
    case Jz: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
		logMessage += "Jz: " + std::to_string(a);
        if (a == 0) {
            args.Ip = detail::Read(instructions, parameters[1], args.Base);
			logMessage += " Ip = " + std::to_string(args.Ip);
			GET_LOGS().emplace_back(logMessage);
            return true;
        }
		logMessage += " Did not jump";
        break;
    }
    case Lt: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& reg = detail::GetRegister(instructions, parameters[2], args.Base);
        reg = a < b ? 1 : 0;
		logMessage += "Lt: Instruction[" + std::to_string(parameters[2].first) + "] = " + std::to_string(a) + " < " + std::to_string(b) + " (" + std::to_string(reg) + ")";
        break;
    }
    case Eq: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
        auto b = detail::Read(instructions, parameters[1], args.Base);
        auto& reg = detail::GetRegister(instructions, parameters[2], args.Base);
        reg = a == b ? 1 : 0;
		logMessage += "Eq: Instructions[" + std::to_string(parameters[2].first) + "] = " + std::to_string(a) + " == " + std::to_string(b) + " (" + std::to_string(reg) + ")";
        break;
    }
    case MoveBase: {
        auto a = detail::Read(instructions, parameters[0], args.Base);
		logMessage += "MoveBase: " + std::to_string(args.Base) + " + " + std::to_string(a);
        args.Base += a;
		logMessage += "(" + std::to_string(args.Base) + ")";
        break;
    }
    }

	GET_LOGS().emplace_back(logMessage);
    args.Ip += parameters.size() + 1;
    return true;
}


constexpr std::vector<s64> ParseInstructions(std::string_view line) {
    return ParseLineAsNumbers<s64>(line);
}

constexpr void RunUntilInteract(std::vector<s64>& instructions, Args& args) {
    auto next = instructions[args.Ip] % 100;
    while (next != OpCode::Set && next != OpCode::Print && next != OpCode::Halt) {
        Apply(instructions, args);
        next = instructions[args.Ip] % 100;
    }
}

constexpr bool HasConsumedAllInput(const Args& args) {
    return args.CurrentInput >= args.Inputs.size() - 1;
}

constexpr bool NeedsInput(const std::vector<s64>& instructions, const Args& args) {
    return instructions[args.Ip] % 100 == OpCode::Set;
}
