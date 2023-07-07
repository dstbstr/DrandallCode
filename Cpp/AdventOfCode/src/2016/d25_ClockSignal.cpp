#include "2016/d25_ClockSignal.h"

SOLUTION(2016, 25) {
    constexpr s32 Unset = 919;

    constexpr auto GenInstruction(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        auto cmd = std::string(s[0]);

        s32 v1 = Unset;
        s32 v2 = Unset;
        size_t i1 = s[1][0] >= 'a' ? s[1][0] - 'a' : Unset;
        size_t i2 = s.size() > 2 && s[2][0] >= 'a' ? s[2][0] - 'a' : Unset;

        if (cmd == "cpy" && i1 == Unset) {
            Constexpr::ParseNumber(s[1], v1);
        }
        else if (cmd == "jnz") {
            if (i1 == Unset) Constexpr::ParseNumber(s[1], v1);
            if (i2 == Unset) Constexpr::ParseNumber(s[2], v2);
        }
        else if (cmd == "tgl" && i1 == Unset) {
            Constexpr::ParseNumber(s[1], v1);
        }

        return [v1, v2, i1, i2, cmd](size_t& ip, std::vector<s64>& regs) {
            s64 outValue = Unset;

            if (cmd == "cpy") regs[i2] = (i1 == Unset ? v1 : regs[i1]);
            else if (cmd == "inc") regs[i1]++;
            else if (cmd == "dec") regs[i1]--;
            else if (cmd == "jnz") {
                auto compare = i1 == Unset ? v1 : regs[i1];
                if (compare != 0) {
                    ip += i2 == Unset ? v2 : regs[i2];
                    return outValue;
                }
            }
            else if (cmd == "out") {
                outValue = i1 == Unset ? v1 : regs[i1];
            }
            ip++;
            return outValue;
        };
    }

    PART_ONE() {
        u64 a = 0;
        auto code = ParseLines(lines, GenInstruction);
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
        return lines[0];
    }

    TESTS() {
        return true;
    }
}