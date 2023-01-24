#pragma once

#include "Common.h"
#include <iostream>

using Registers = std::array<u64, 6>;
struct Instruction {
    std::string_view Op;
    u64 A;
    u64 B;
    u64 C;

    friend std::ostream& operator<<(std::ostream& os, const Instruction& inst) {
        os << inst.Op << " " << inst.A << " " << inst.B << " " << inst.C;
        return os;
    }
};

constexpr void Apply(const Instruction& inst, size_t ipReg, Registers& regs) {
    if (inst.Op == "addr") regs[inst.C] = regs[inst.A] + regs[inst.B];
    else if (inst.Op == "addi") regs[inst.C] = regs[inst.A] + inst.B;
    else if (inst.Op == "mulr") regs[inst.C] = regs[inst.A] * regs[inst.B];
    else if (inst.Op == "muli") regs[inst.C] = regs[inst.A] * inst.B;
    else if (inst.Op == "banr") regs[inst.C] = regs[inst.A] & regs[inst.B];
    else if (inst.Op == "bani") regs[inst.C] = regs[inst.A] & inst.B;
    else if (inst.Op == "borr") regs[inst.C] = regs[inst.A] | regs[inst.B];
    else if (inst.Op == "bori") regs[inst.C] = regs[inst.A] | inst.B;
    else if (inst.Op == "setr") regs[inst.C] = regs[inst.A];
    else if (inst.Op == "seti") regs[inst.C] = inst.A;
    else if (inst.Op == "gtir") regs[inst.C] = inst.A > regs[inst.B] ? 1 : 0;
    else if (inst.Op == "gtri") regs[inst.C] = regs[inst.A] > inst.B ? 1 : 0;
    else if (inst.Op == "gtrr") regs[inst.C] = regs[inst.A] > regs[inst.B] ? 1 : 0;
    else if (inst.Op == "eqir") regs[inst.C] = inst.A == regs[inst.B] ? 1 : 0;
    else if (inst.Op == "eqri") regs[inst.C] = regs[inst.A] == inst.B ? 1 : 0;
    else if (inst.Op == "eqrr") regs[inst.C] = regs[inst.A] == regs[inst.B] ? 1 : 0;

    regs[ipReg]++;
}

constexpr std::vector<Instruction> ParseInstructions(const std::vector<std::string>& lines) {
    std::vector<Instruction> instructions;
    for (auto i = 1; i < lines.size(); i++) {
        auto split = Constexpr::Split(lines[i], " ");
        u64 a, b, c;
        Constexpr::ParseNumber(split[1], a);
        Constexpr::ParseNumber(split[2], b);
        Constexpr::ParseNumber(split[3], c);
        instructions.push_back({ split[0], a, b, c });
    }

    return instructions;
}

auto PartOne(const std::vector<std::string>& lines) {
    Registers regs{};
    size_t ipReg;
    Constexpr::ParseNumber(Constexpr::Split(lines[0], " ")[1], ipReg);
    auto instructions = ParseInstructions(lines);

    while (true) {
        auto instruction = regs[ipReg];
        if (instruction >= instructions.size()) break;
        if (instruction == 28) {
            return regs[4];
        }
        Apply(instructions[instruction], ipReg, regs);
    }

    return lines.size();
}

auto PartTwo(const std::vector<std::string>& lines) {
    Registers regs{};
    size_t ipReg;
    Constexpr::ParseNumber(Constexpr::Split(lines[0], " ")[1], ipReg);
    auto instructions = ParseInstructions(lines);
    std::unordered_set<u64> seen;
    u64 previous = 0;

    while (true) {
        auto instruction = regs[ipReg];
        if (instruction >= instructions.size()) break;
        if (instruction == 28) {
            auto val = regs[4];
            if (seen.contains(val)) {
                return previous;
            }
            seen.insert(val);
            previous = val;

            //std::cout << val << " (" << seen.size() << ")\n";
        }
        Apply(instructions[instruction], ipReg, regs);
    }

    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {

    };

    if (PartOne(lines) != 0) return false;
    return true;
}