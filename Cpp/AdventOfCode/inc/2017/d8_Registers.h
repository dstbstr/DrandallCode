#pragma once

#include "Common.h"

static constexpr size_t RegisterCount = Constexpr::FromBase26("zzz"sv);

using Registers = std::array<s32, RegisterCount>;

constexpr bool EvaluateCondition(std::string_view condRegister, std::string_view comparison, std::string_view operand, const Registers& registers) {
    auto regVal = registers[Constexpr::FromBase26(condRegister)];
    s32 compareValue;
    Constexpr::ParseNumber(operand, compareValue);
    return Constexpr::Eval(regVal, compareValue, comparison);
}

constexpr void ApplyInstruction(const std::string& line, Registers& registers) {
    auto split = Constexpr::Split(line, " ");
    auto targetRegister = split[0];
    auto command = split[1];
    auto deltaStr = split[2];
    auto condRegister = split[4];
    auto comparison = split[5];
    auto operand = split[6];

    if (EvaluateCondition(condRegister, comparison, operand, registers)) {
        s32 delta;
        Constexpr::ParseNumber(deltaStr, delta);
        if (command == "inc") {
            registers[Constexpr::FromBase26(targetRegister)] += delta;
        }
        else {
            registers[Constexpr::FromBase26(targetRegister)] -= delta;
        }
    }
}

constexpr auto PartOne(const std::vector<std::string>& lines) {
    Registers regs;
    regs.fill(0);
    for (const auto& line : lines) {
        ApplyInstruction(line, regs);
    }

    return Constexpr::FindMax(regs);
}

static_assert(PartOne({ "b inc 5 if a > 1", "a inc 1 if b < 5", "c dec -10 if a >= 1", "c inc -20 if c == 10" }) == 1);

constexpr auto PartTwo(const std::vector<std::string>& lines) {
    Registers regs;
    regs.fill(0);
    s32 max = 0;
    for (const auto& line : lines) {
        ApplyInstruction(line, regs);
        max = std::max(max, Constexpr::FindMax(regs));
    }

    return max;
}

static_assert(PartTwo({ "b inc 5 if a > 1", "a inc 1 if b < 5", "c dec -10 if a >= 1", "c inc -20 if c == 10" }) == 10);

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}