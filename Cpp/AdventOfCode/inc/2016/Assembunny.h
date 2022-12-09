#pragma once

#include "Common.h"

constexpr s32 GetValue(std::string_view sv, const std::vector<s32>& registers) {
    if (sv[0] <= 'd' && sv[0] >= 'a') {
        return registers[sv[0] - 'a'];
    }
    else {
        s32 result;
        Constexpr::ParseNumber(sv, result);
        return result;
    }
}

static_assert(GetValue("a", { 12, 24 }) == 12);
static_assert(GetValue("b", { 12, 24 }) == 24);
static_assert(GetValue("4", { 12, 24 }) == 4);

constexpr void ApplyInstruction(const std::string& line, u32& instruction, std::vector<s32>& registers) {
    auto split = Constexpr::Split(line, " ");
    if (split[0] == "cpy") {
        auto value = GetValue(split[1], registers);
        registers[split[2][0] - 'a'] = value;
        instruction++;
    }
    else if (split[0] == "inc") {
        registers[split[1][0] - 'a']++;
        instruction++;
    }
    else if (split[0] == "dec") {
        registers[split[1][0] - 'a']--;
        instruction++;
    }
    else if (split[0] == "jnz") {
        s32 compare = GetValue(split[1], registers);
        if (compare != 0) {
            s32 jump = GetValue(split[2], registers);
            instruction += jump;
        }
        else {
            instruction++;
        }
    }
}

constexpr void Execute(const std::vector<std::string>& lines, std::vector<s32>& registers) {
    u32 instruction = 0;
    while (instruction < lines.size()) {
        ApplyInstruction(lines[instruction], instruction, registers);
    }
}

std::string Run(const std::vector<std::string>& lines) {
    std::vector<s32> registers{ 0, 0, 1, 0 };
    Execute(lines, registers);

    return ToString(registers[0]);
}

bool RunTests() {
    std::vector<std::string> lines;
    lines.push_back("cpy 41 a");
    lines.push_back("inc a");
    lines.push_back("inc a");
    lines.push_back("dec a");
    lines.push_back("jnz a 2");
    lines.push_back("dec a");

    std::vector<s32> registers{ 0, 0, 0, 0 };
    Execute(lines, registers);
    if (registers[0] != 42) return false;

    return true;
}