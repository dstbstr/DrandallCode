#include "2018/d19_Comp2.h"

SOLUTION(2018, 19) {
    using Regs = std::array<s32, 6>;
    struct Instruction {
        std::string_view Op;
        u8 A;
        u8 B;
        u8 C;
    };

    enum struct Op {Addr, Addi, Mulr, Muli, Banr, Bani, Borr, Bori, Setr, Seti, Gtir, Gtrr, Gtri, Eqir, Eqri, Eqrr};
    constexpr auto GenInst(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        auto opStr = s[0];
        s32 a, b, c;
        Constexpr::ParseNumber(s[1], a);
        Constexpr::ParseNumber(s[2], b);
        Constexpr::ParseNumber(s[3], c);

        Op op;
        if (opStr == "addr") op = Op::Addr;
        else if (opStr == "addi") op = Op::Addi;
        else if (opStr == "mulr") op = Op::Mulr;
        else if (opStr == "muli") op = Op::Muli;
        else if (opStr == "banr") op = Op::Banr;
        else if (opStr == "bani") op = Op::Bani;
        else if (opStr == "borr") op = Op::Borr;
        else if (opStr == "bori") op = Op::Bori;
        else if (opStr == "setr") op = Op::Setr;
        else if (opStr == "seti") op = Op::Seti;
        else if (opStr == "gtir") op = Op::Gtir;
        else if (opStr == "gtrr") op = Op::Gtrr;
        else if (opStr == "gtri") op = Op::Gtri;
        else if (opStr == "eqir") op = Op::Eqir;
        else if (opStr == "eqri") op = Op::Eqri;
        else if (opStr == "eqrr") op = Op::Eqrr;

        return [a, b, c, op](Regs& regs) {
            switch (op) {
                case Op::Addr: regs[c] = regs[a] + regs[b]; break;
                case Op::Addi: regs[c] = regs[a] + b; break;
                case Op::Mulr: regs[c] = regs[a] * regs[b]; break;
                case Op::Muli: regs[c] = regs[a] * b; break;
                case Op::Banr: regs[c] = regs[a] & regs[b]; break;
                case Op::Bani: regs[c] = regs[a] & b; break;
                case Op::Borr: regs[c] = regs[a] | regs[b]; break;
                case Op::Bori: regs[c] = regs[a] | b; break;
                case Op::Setr: regs[c] = regs[a]; break;
                case Op::Seti: regs[c] = a; break;
                case Op::Gtir: regs[c] = a > regs[b] ? 1 : 0; break;
                case Op::Gtrr: regs[c] = regs[a] > regs[b] ? 1 : 0; break;
                case Op::Gtri: regs[c] = regs[a] > b ? 1 : 0; break;
                case Op::Eqir: regs[c] = a == regs[b] ? 1 : 0; break;
                case Op::Eqri: regs[c] = regs[a] == b ? 1 : 0; break;
                case Op::Eqrr: regs[c] = regs[a] == regs[b] ? 1 : 0; break;
            }
        };

    }

    constexpr size_t GetIpReg(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        size_t result;
        Constexpr::ParseNumber(s[1], result);
        return result;
    }

    PART_ONE() {
        auto ipReg = GetIpReg(lines[0]);
        auto copy = std::vector<std::string>(lines.begin() + 1, lines.end());
        auto inst = ParseLines(copy, GenInst);
        Regs regs{};
        s32& ip = regs[ipReg];
        while (ip < inst.size()) {
            inst[ip](regs);
            ip++;
        }
        
        return Constexpr::ToString(regs[0]);
    }

    PART_TWO() {
        auto ipReg = GetIpReg(lines[0]);
        auto copy = std::vector<std::string>(lines.begin() + 1, lines.end());
        auto inst = ParseLines(copy, GenInst);
        Regs regs{};
        regs[0] = 1;

        s32& ip = regs[ipReg];
        for (auto i = 0; i < 17; i++) {
            inst[ip](regs);
            ip++;
        }
        auto targetNumber = regs[4];
        auto divisors = Constexpr::GetDivisors(targetNumber);
        return Constexpr::ToString(std::accumulate(divisors.begin(), divisors.end(), 0));
    }

    TESTS() {
        //std::vector<std::string> lines = {
        //    "#ip 0",
        //    "seti 5 0 1",
        //    "seti 6 0 2",
        //    "addi 0 1 0",
        //    "addr 1 2 3",
        //    "setr 1 0 0",
        //    "seti 8 0 4",
        //    "seti 9 0 5"
        //};
        //
        //if (PartOne(lines) != "7") return false;

        return true;
    }
}