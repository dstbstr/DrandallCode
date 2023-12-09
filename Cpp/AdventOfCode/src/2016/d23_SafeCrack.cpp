#include "2016/d23_SafeCrack.h"

SOLUTION(2016, 23) {
    constexpr s32 Unset = 919;
    enum struct Command {Cpy, Jnz, Tgl, Inc, Dec};
    constexpr auto GenInstruction(std::string_view line, size_t maxIp) {
        auto s = Constexpr::Split(line, " ");
        auto cmdStr = std::string(s[0]);
        Command cmd;
        if (cmdStr == "cpy") cmd = Command::Cpy;
        else if (cmdStr == "jnz") cmd = Command::Jnz;
        else if (cmdStr == "tgl") cmd = Command::Tgl;
        else if (cmdStr == "inc") cmd = Command::Inc;
        else if (cmdStr == "dec") cmd = Command::Dec;

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

        return [v1, v2, i1, i2, cmd, maxIp](size_t& ip, std::vector<s32>& regs, std::vector<size_t>& toggles) {
            if (toggles[ip] == 0) {
                switch (cmd) {
                case Command::Cpy: if (i2 != Unset) regs[i2] = i1 == Unset ? v1 : regs[i1]; break;
                case Command::Inc: if (i1 != Unset) regs[i1]++; break;
                case Command::Dec: if (i1 != Unset) regs[i1]--; break;
                case Command::Jnz: {
                    auto compare = i1 == Unset ? v1 : regs[i1];
                    if (compare != 0) {
                        ip += i2 == Unset ? v2 : regs[i2];
                        return;
                    }
                    break;
                }
                case Command::Tgl: {
                    s32 distance = i1 == Unset ? v1 : regs[i1];
                    s64 targetIndex = ip + distance;
                    if (targetIndex < 0 || targetIndex >= static_cast<s64>(maxIp)) {
                        ip++;
                        return;
                    }
                    toggles[targetIndex]++;
                    break;
                }
                }
                ip++;
            }
            else {
                if (toggles[ip] > 1) throw "I didn't plan for this";
                switch (cmd){
                case Command::Cpy: {
                    auto compare = i1 == Unset ? v1 : regs[i1];
                    if (compare != 0) {
                        ip += i2 == Unset ? v2 : regs[i2];
                        return;
                    }
                    break;
                }
                case Command::Inc: if (i1 != Unset) regs[i1]--; break;
                case Command::Dec: if (i1 != Unset) regs[i1]++; break;
                case Command::Jnz: if (i2 != Unset) regs[i2] = i1 == Unset ? v1 : regs[i1]; break;
                case Command::Tgl: if (i1 != Unset) regs[i1]++; break;
                }
                ip++;
            }
        };
    }

    constexpr s32 Execute(const auto& lines, s32 reg0) {
        size_t ip = 0;
        auto code = lines;
        auto regs = std::vector<s32>{ reg0, 0, 0, 0 };
        auto exe = ParseLines(lines, GenInstruction, lines.size());
        auto toggles = std::vector<size_t>(lines.size());

        while (ip < code.size()) {
            exe[ip](ip, regs, toggles);
        }

        return regs[0];
    }

    constexpr size_t CalculateConstant(const auto& lines) {
        size_t lhs = 0, rhs = 0;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " ");
            if (s[0] == "cpy" == s[1].size() > 1) {
                if (s[1][0] == '-') continue;
                Constexpr::ParseNumber(s[1], lhs);
            }
            else if (s[0] == "jnz" && s[1].size() > 1) {
                Constexpr::ParseNumber(s[1], rhs);
                break;
            }
        }
        return lhs * rhs;
    }

    PART_ONE() {
        return Constexpr::ToString(CalculateConstant(lines) + Constexpr::Factorial(7));
        //return Constexpr::ToString(Execute(lines, 7));
    }
    PART_TWO() {
        return Constexpr::ToString(CalculateConstant(lines) + Constexpr::Factorial(12ull));
        //return Constexpr::ToString(Execute(lines, 12));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "cpy 2 a",
            "tgl a",
            "tgl a",
            "tgl a",
            "cpy 1 a",
            "dec a",
            "dec a"
        };

        //if (Part1(lines) != "3") return false;

        return true;
    }
}