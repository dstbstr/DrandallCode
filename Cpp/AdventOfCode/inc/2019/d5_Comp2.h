#pragma once

#include "Common.h"

enum OpCode { Add = 1, Mul = 2, Set = 3, Print = 4, Jnz = 5, Jz = 6, Lt = 7, Eq = 8, Halt = 99 };
enum ParamMode { Position = 0, Immediate = 1};

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

constexpr s64 Read(const std::vector<s64>& instructions, const std::pair<s64, ParamMode>& param) {
    const auto& [value, mode] = param;

    if (mode == ParamMode::Immediate) {
        return value;
    }
    else {
        return instructions[value];
    }
}

constexpr s64& GetRegister(std::vector<s64>& instructions, const std::pair<s64, ParamMode>& param) {
    return instructions[param.first];
}

constexpr bool Apply(std::vector<s64>& instructions, size_t& index, s64 input, s64& output) {
    OpCode op;
    std::vector<ParamMode> modes;
    ParseInstruction(instructions[index], op, modes);
    std::vector<std::pair<s64, ParamMode>> parameters;
    for (auto i = 0; i < ArgCount(op); i++) {
        auto pIndex = index + i + 1;
        auto val = instructions[pIndex];
        auto mode = modes.size() <= i ? ParamMode::Position : modes[i];
        parameters.push_back(std::make_pair(val, mode));
    }

    switch (op)
    {
    case Halt: return false;
    case Add: {
        auto a = Read(instructions, parameters[0]);
        auto b = Read(instructions, parameters[1]);
        auto& c = GetRegister(instructions, parameters[2]);
        c = a + b;
        break;
    }
    case Mul: {
        auto a = Read(instructions, parameters[0]);
        auto b = Read(instructions, parameters[1]);
        auto& c = GetRegister(instructions, parameters[2]);
        c = a * b;
        break;
    }
    case Set: {
        auto& reg = GetRegister(instructions, parameters[0]);
        reg = input;
        break;
    }
    case Print: {
        output = Read(instructions, parameters[0]);
        break;
    }
    case Jnz: {
        auto a = Read(instructions, parameters[0]);
        if (a != 0) {
            index = Read(instructions, parameters[1]);
            return true;
        }
        break;
    }
    case Jz: {
        auto a = Read(instructions, parameters[0]);
        if (a == 0) {
            index = Read(instructions, parameters[1]);
            return true;
        }
        break;
    }
    case Lt: {
        auto a = Read(instructions, parameters[0]);
        auto b = Read(instructions, parameters[1]);
        auto& reg = GetRegister(instructions, parameters[2]);
        reg = a < b ? 1 : 0;
        break;
    }
    case Eq: {
        auto a = Read(instructions, parameters[0]);
        auto b = Read(instructions, parameters[1]);
        auto& reg = GetRegister(instructions, parameters[2]);
        reg = a == b ? 1 : 0;
        break;
    }
    }

    index += parameters.size() + 1;
    return true;
}

constexpr bool TestApply(std::vector<s64> start, std::vector<s64> expectedOutput) {
    s64 input = 0;
    s64 output = 0;
    size_t index = 0;
    while (Apply(start, index, input, output));

    return start == expectedOutput;
}

constexpr bool TestApply(std::vector<s64> start, s64 input, s64 expectedOutput) {
    s64 actualOutput = 0;
    size_t index = 0;
    while (Apply(start, index, input, actualOutput));

    return actualOutput == expectedOutput;
}

static_assert(TestApply({ 1,0,0,0,99 }, { 2,0,0,0,99 }));
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

constexpr std::vector<s64> ParseLine(const std::string& line) {
    std::vector<s64> result;
    auto split = Constexpr::Split(line, ",");
    for (const auto& e : split) {
        s64 val;
        Constexpr::ParseNumber(e, val);
        result.push_back(val);
    }

    return result;
}

bool PartOne(const std::string& line, s64& result) {
    size_t index = 0;
    auto instructions = ParseLine(line);
    s64 input = 1;

    while (Apply(instructions, index, input, result)) {
        if (result != 0) {
            return instructions[index] % 100 == Halt;
        }
    }

    return false;
}

auto PartTwo(const std::string& line, s64& result) {
    size_t index = 0;
    auto instructions = ParseLine(line);
    s64 input = 5;

    while (Apply(instructions, index, input, result));
    return true;
}

std::string Run(const std::vector<std::string>& lines) {
    s64 result = 0;
    //if (PartOne(lines[0], result)) {
    if (PartTwo(lines[0], result)) {
        return ToString(result);
    }
    else {
        return "Failure";
    }
}

bool RunTests() {
    if(!TestApply({ 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9 }, 0, 0)) return false;
    if(!TestApply({ 3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9 }, 24, 1)) return false;
    /*
    std::vector<s64> inst = { 1,1,1,4,99,5,6,0,99 };
    size_t index = 0;
    s64 input = 0;
    s64 output = 0;
    while (Apply(inst, index, input, output));
    if (inst[0] != 30) return false;
    */
    return true;
}