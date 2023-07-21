#include "2018/d21_Comp3.h"

SOLUTION(2018, 21) {
    using Regs = std::array<u64, 6>;

    enum struct Op { Addr, Addi, Mulr, Muli, Banr, Bani, Borr, Bori, Setr, Seti, Gtir, Gtrr, Gtri, Eqir, Eqri, Eqrr };
    constexpr auto GenInst(const std::string & line) {
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

    constexpr size_t GetIpReg(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        size_t result;
        Constexpr::ParseNumber(s[1], result);
        return result;
    }


    constexpr auto Mask = 0xFFFFFF;
    constexpr u64 Step(u64 previous, u64 previousMask, u64 initialValue, u64 multiplier) {
        previous |= previousMask;
        u64 b = initialValue;
        b = (b + (previous & 0xFF)) & Mask;
        b = (b * multiplier) & Mask;
        b = (b + ((previous >> 8) & 0xFF)) & Mask;
        b = (b * multiplier) & Mask;
        b = (b + ((previous >> 16) & 0xFF)) & Mask;
        b = (b * multiplier) & Mask;
        return b;
    }

    constexpr void GatherValues(const std::vector<std::string>& lines, u64& outPreviousMask, u64& outInitialValue, u64& outMultiplier) {
        auto s1 = Constexpr::Split(lines[7], " ");
        Constexpr::ParseNumber(s1[2], outPreviousMask);

        auto s2 = Constexpr::Split(lines[8], " ");
        Constexpr::ParseNumber(s2[1], outInitialValue);

        auto s3 = Constexpr::Split(lines[12], " ");
        Constexpr::ParseNumber(s3[2], outMultiplier);
    }

    PART_ONE() {
        u64 previousMask, initialValue, multiplier;
        GatherValues(lines, previousMask, initialValue, multiplier);

        return Constexpr::ToString(Step(0, previousMask, initialValue, multiplier));
        /*
        auto ipReg = GetIpReg(lines[0]);
        Regs regs{};
        size_t& ip = regs[ipReg];
        std::vector<std::string> copy(lines.begin() + 1, lines.end());
        auto inst = ParseLines(copy, GenInst);

        while (true) {
            if (ip >= inst.size()) break;
            if (ip == 28) return Constexpr::ToString(regs[4]);
            inst[ip](regs);
            ip++;
        }

        return "Not Found";
        */
    }


    PART_TWO() {
        u64 previousMask, initialValue, multiplier;
        GatherValues(lines, previousMask, initialValue, multiplier);

        Constexpr::SmallSet<u64> seen;
        u64 n = 0;
        u64 n2 = 0;
        while (true) {
            n2 = Step(n, previousMask, initialValue, multiplier);
            if (!seen.insert(n2)) {
                return Constexpr::ToString(n);
            }
            n = n2;
        }
        return "Not Found";
        /*
        auto ipReg = GetIpReg(lines[0]);
        Regs regs{};
        size_t& ip = regs[ipReg];
        std::vector<std::string> copy(lines.begin() + 1, lines.end());
        auto inst = ParseLines(copy, GenInst);

        Constexpr::Set<u64> seen;
        u64 previous = 0;
        while (true) {
            if (ip >= inst.size()) break;
            if (ip == 28) {
                auto val = regs[4];
                auto newVal = Step(previous);
                if (val != newVal) {
                    break; //bummer
                }
                if (!seen.insert(val)) {
                    return Constexpr::ToString(previous);
                }
                previous = val;
            }
            inst[ip](regs);
            ip++;
        }

        return "Not Found";
        */
    }

    TESTS() {
        return true;
    }
}