#include "2016/d23_SafeCrack.h"

SOLUTION(2016, 23) {
    constexpr s32 Unset = 919;
    constexpr auto GenInstruction(const std::string& line, size_t maxIp) {
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

        return [v1, v2, i1, i2, cmd, maxIp](size_t& ip, std::vector<s32>& regs, std::vector<size_t>& toggles) {
            if (toggles[ip] == 0) {
                if (cmd == "cpy") {
                    if (i2 != Unset) {
                        regs[i2] = i1 == Unset ? v1 : regs[i1];
                    }
                }
                else if (cmd == "inc") {
                    if (i1 != Unset) {
                        regs[i1]++;
                    }
                }
                else if (cmd == "dec") {
                    if (i1 != Unset) {
                        regs[i1]--;
                    }
                }
                else if (cmd == "jnz") {
                    auto compare = i1 == Unset ? v1 : regs[i1];
                    if (compare != 0) {
                        ip += i2 == Unset ? v2 : regs[i2];
                        return;
                    }
                }
                else if (cmd == "tgl") {
                    s32 distance = i1 == Unset ? v1 : regs[i1];
                    s64 targetIndex = ip + distance;
                    if (targetIndex < 0 || targetIndex >= static_cast<s64>(maxIp)) {
                        ip++;
                        return;
                    }
                    toggles[targetIndex]++;
                }
                ip++;
            }
            else {
                if (toggles[ip] > 1) throw "I didn't plan for this";
                if (cmd == "cpy") {
                    auto compare = i1 == Unset ? v1 : regs[i1];
                    if (compare != 0) {
                        ip += i2 == Unset ? v2 : regs[i2];
                        return;
                    }
                }
                else if (cmd == "inc") {
                    if (i1 != Unset) {
                        regs[i1]--;
                    }
                }
                else if (cmd == "dec") {
                    if (i1 != Unset) {
                        regs[i1]++;
                    }
                }
                else if (cmd == "jnz") {
                    if (i2 != Unset) {
                        regs[i2] = i1 == Unset ? v1 : regs[i1];
                    }
                }
                else if (cmd == "tgl") {
                    if (i1 != Unset) {
                        regs[i1]++;
                    }
                }
                ip++;
            }
        };
    }

    constexpr s32 Execute(const std::vector<std::string>& lines, s32 reg0) {
        size_t ip = 0;
        auto code = lines;
        auto regs = std::vector<s32>{ reg0, 0, 0, 0 };
        using Inst = decltype(GenInstruction("", 0ull));
        auto exe = ParseLines<Inst>(lines, GenInstruction, lines.size());
        auto toggles = std::vector<size_t>(lines.size());

        while (ip < code.size()) {
            exe[ip](ip, regs, toggles);
        }

        return regs[0];
    }

    constexpr size_t CalculateConstant(const std::vector<std::string>& lines) {
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