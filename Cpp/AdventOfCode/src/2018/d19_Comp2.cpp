#include "2018/d19_Comp2.h"

SOLUTION(2018, 19) {
    using Registers = std::array<s32, 6>;
    struct Instruction {
        std::string_view Op;
        u8 A;
        u8 B;
        u8 C;
    };

    constexpr void Apply(const Instruction & inst, size_t ipReg, Registers & regs) {
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

    constexpr std::vector<Instruction> ParseInstructions(const std::vector<std::string>&lines) {
        std::vector<Instruction> instructions;
        for (auto i = 1; i < lines.size(); i++) {
            auto split = Constexpr::Split(lines[i], " ");
            u8 a, b, c;
            Constexpr::ParseNumber(split[1], a);
            Constexpr::ParseNumber(split[2], b);
            Constexpr::ParseNumber(split[3], c);
            instructions.push_back({ split[0], a, b, c });
        }

        return instructions;
    }

    auto Part1(const std::vector<std::string>&lines) {
        Registers regs{};
        size_t ipReg;
        Constexpr::ParseNumber(Constexpr::Split(lines[0], " ")[1], ipReg);

        auto instructions = ParseInstructions(lines);
        while (true) {
            auto instruction = regs[ipReg];
            if (instruction >= instructions.size()) break;
            Apply(instructions[instruction], ipReg, regs);
        }

        return regs[0];
    }

#include <iostream>

    auto Part2(const std::vector<std::string>&) {
        u32 targetNumber = 10551275;
        auto divisors = Constexpr::GetDivisors(targetNumber);
        return std::accumulate(divisors.begin(), divisors.end(), 0);
        //return Constexpr::Sum(divisors);

        /*
        Registers regs{};
        size_t ipReg;
        Constexpr::ParseNumber(Constexpr::Split(lines[0], " ")[1], ipReg);
        regs[0] = 1;

        s32 previousZero = 1;
        auto instructions = ParseInstructions(lines);
        while (true) {
            auto instruction = regs[ipReg];
            if (regs[0] != previousZero) {
                std::cout << regs[0] << " " << regs[1] << " [" << regs[2] << "] " << regs[3] << " " << regs[4] << " " << regs[5] << '\n';
                previousZero = regs[0];
            }
            if (instruction >= instructions.size()) break;
            Apply(instructions[instruction], ipReg, regs);
        }

        return regs[0];
        */
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "#ip 0",
            "seti 5 0 1",
            "seti 6 0 2",
            "addi 0 1 0",
            "addr 1 2 3",
            "setr 1 0 0",
            "seti 8 0 4",
            "seti 9 0 5"
        };

        if (Part1(lines) != 7) return false;
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