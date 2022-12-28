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

constexpr void ApplyInstruction(const std::string& line, std::vector<std::string>& program, u32& instruction, std::vector<s32>& registers) {
    auto split = Constexpr::Split(line, " ");
    if (split[0] == "cpy") {
        if (split[2][0] >= 'a') {
            auto value = GetValue(split[1], registers);
            registers[split[2][0] - 'a'] = value;
        }
        instruction++;
    }
    else if (split[0] == "inc") {
        if (split[1][0] >= 'a') {
            registers[split[1][0] - 'a']++;
        }
        instruction++;
    }
    else if (split[0] == "dec") {
        if (split[1][0] >= 'a') {
            registers[split[1][0] - 'a']--;
        }
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
    else if (split[0] == "tgl") {
        s32 distance = GetValue(split[1], registers);
        if (distance < 0 && static_cast<u32>(Constexpr::Abs(distance)) > instruction) {
            instruction++;
            return;
        }
        else if (distance + instruction >= program.size()) {
            instruction++;
            return;
        }

        auto& targetInstruction = program[instruction + distance];
        auto split2 = Constexpr::Split(targetInstruction, " ");
        if (split2[0] == "inc") {
            split2[0] = "dec";
        }
        else if (split2[0] == "jnz") {
            split2[0] = "cpy";
        }
        else if (split2.size() == 2) {
            split2[0] = "inc";
        }
        else if (split2.size() == 3) {
            split2[0] = "jnz";
        }

        targetInstruction = Constexpr::JoinVec(" ", split2);
        instruction++;
    }
}

constexpr s32 Execute(const std::vector<std::string>& lines, std::vector<s32>& registers) {
    u32 instruction = 0;
    std::vector<std::string> program = lines;
    while (instruction < program.size()) {
        ApplyInstruction(program[instruction], program, instruction, registers);
    }

    return registers[0];
}
s32 PartOne(const std::vector<std::string>& lines) {
    std::vector<s32> registers = { 7, 0, 0, 0 };
    return Execute(lines, registers);
}

u32 PartTwo(const std::vector<std::string>& lines) {
    std::vector<s32> registers = { 12, 0, 0, 0 };
    return Execute(lines, registers);
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines)); //super slow, but works
}

bool RunTests() {
    std::vector<std::string> lines = {
        "cpy 2 a",
        "tgl a",
        "tgl a",
        "tgl a",
        "cpy 1 a",
        "dec a",
        "dec a"
    };

    if (PartOne(lines) != 3) return false;

    return true;
}