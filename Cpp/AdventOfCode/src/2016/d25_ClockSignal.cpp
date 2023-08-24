#include "2016/d25_ClockSignal.h"

SOLUTION(2016, 25) {
    constexpr s32 Unset = 919;
    enum struct Command { Cpy, Jnz, Tgl, Inc, Dec, Out};
    constexpr auto GenInstruction(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        auto cmdStr = std::string(s[0]);
        Command cmd = Command::Out;

        if (cmdStr == "cpy") cmd = Command::Cpy;
        else if (cmdStr == "jnz") cmd = Command::Jnz;
        else if (cmdStr == "tgl") cmd = Command::Tgl;
        else if (cmdStr == "inc") cmd = Command::Inc;
        else if (cmdStr == "dec") cmd = Command::Dec;
        else if (cmdStr == "out") cmd = Command::Out;

        s32 v1 = Unset;
        s32 v2 = Unset;
        size_t i1 = s[1][0] >= 'a' ? s[1][0] - 'a' : Unset;
        size_t i2 = s.size() > 2 && s[2][0] >= 'a' ? s[2][0] - 'a' : Unset;

        if (cmd == Command::Cpy && i1 == Unset) {
            Constexpr::ParseNumber(s[1], v1);
        }
        else if (cmd == Command::Jnz) {
            if (i1 == Unset) Constexpr::ParseNumber(s[1], v1);
            if (i2 == Unset) Constexpr::ParseNumber(s[2], v2);
        }
        else if (cmd == Command::Tgl && i1 == Unset) {
            Constexpr::ParseNumber(s[1], v1);
        }

        return [v1, v2, i1, i2, cmd](size_t& ip, std::vector<s64>& regs) {
            s64 outValue = Unset;
            switch (cmd) {
            case Command::Cpy: regs[i2] = (i1 == Unset ? v1 : regs[i1]); break;
            case Command::Inc: regs[i1]++; break;
            case Command::Dec: regs[i1]--; break;
            case Command::Jnz: {
                auto compare = i1 == Unset ? v1 : regs[i1];
                if (compare != 0) {
                    ip += i2 == Unset ? v2 : regs[i2];
                    return outValue;
                }
                break;
            }
            case Command::Out: outValue = i1 == Unset ? v1 : regs[i1]; break;
            }
            ip++;
            return outValue;
        };
    }

    PART_ONE() {
        u64 a = 0;
        auto code = ParseLines(Lines, GenInstruction);
        while (true) {
            std::vector<s64> regs = { 0, 0, 0, 0 };
            size_t ip = 0;
            regs[0] = a++;
            s64 lastOutput = 1;
            u32 outLength = 0;
            bool found = true;
            s64 out = Unset;

            while (outLength < 50) {
                out = code[ip](ip, regs);
                if (out != Unset) {
                    if (out == lastOutput) {
                        found = false;
                        break;
                    }
                    else {
                        outLength++;
                        lastOutput = out;
                    }
                }
            }

            if (found) {
                return Constexpr::ToString(a - 1);
            }
        }
        return "Not Found";
    }

    PART_TWO() {
        return "Merry Christmas";
    }

    TESTS() {
        return true;
    }
}