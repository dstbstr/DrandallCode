#include "2016/d25_ClockSignal.h"

SOLUTION(2016, 25) {
    constexpr u64 GetValue(std::string_view sv, const std::vector<u64>&registers) {
        if (sv[0] <= 'd' && sv[0] >= 'a') {
            return registers[sv[0] - 'a'];
        }
        else {
            s32 result;
            Constexpr::ParseNumber(sv, result);
            return result;
        }
    }

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

    /*
    constexpr void ApplyInstruction(const std::string & line, u32 & instruction, std::vector<u64>&registers, std::string & transmission) {
        auto split = Constexpr::Split(line, " ");
        if (split[0] == "cpy") {
            auto value = GetValue(split[1], registers);
            registers[split[2][0] - 'a'] = value;
        }
        else if (split[0] == "inc") {
            registers[split[1][0] - 'a']++;
        }
        else if (split[0] == "dec") {
            registers[split[1][0] - 'a']--;
        }
        else if (split[0] == "jnz") {
            auto compare = GetValue(split[1], registers);
            if (compare != 0) {
                auto jump = GetValue(split[2], registers);
                instruction += static_cast<u32>(jump);
                return;
            }
        }
        else if (split[0] == "out") {
            transmission += ToString(GetValue(split[1], registers));
        }
        else if (split[0] == "add") {
            auto val = GetValue(split[2], registers);
            registers[split[1][0] - 'a'] += val;
        }
        else if (split[0] == "sub") {
            auto val = GetValue(split[2], registers);
            registers[split[1][0] - 'a'] -= val;
        }
        else if (split[0] == "mul") {
            auto lhs = GetValue(split[1], registers);
            auto rhs = GetValue(split[2], registers);
            registers[split[3][0] - 'a'] += lhs * rhs;
        }
        else if (split[0] == "div") {
            auto lhs = GetValue(split[1], registers);
            auto rhs = GetValue(split[2], registers);
            registers[split[1][0] - 'a'] = lhs / rhs;
            registers[split[3][0] - 'a'] = lhs % rhs;
        }
        instruction++;
    }
    */
    PART_ONE() {
        /*
        using Inst = decltype(GenInstruction(""));
        auto code = ParseLines<Inst>(lines, GenInstruction);

        std::vector<s64> regs = { 192, 0, 0, 0 };
        size_t ip = 0;
        s64 lastOutput = 1;
        u32 outLength = 0;
        s64 out = Unset;
        while (outLength < 50) {
            out = code[ip](ip, regs);
            if (out == Unset) continue;
            if (out == lastOutput) {
                return "Not Found";
            }
            lastOutput = out;
            outLength++;
        }

        return "192";
        */

        u64 a = 0;
        using Inst = decltype(GenInstruction(""));
        auto code = ParseLines<Inst>(lines, GenInstruction);
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
        return lines[0];
        
        /*
        u64 a = 0;
        while (true) {
            std::vector<u64> registers = { 0, 0, 0, 0 };
            u32 instruction = 0;
            registers[0] = a++;
            bool expectedOutput = false;
            u32 transmissionCount = 0;
            bool isValid = true;
            while (transmissionCount < 50) {
                std::string transmission = "";
                ApplyInstruction(lines[instruction], instruction, registers, transmission);
                if (!transmission.empty()) {
                    transmissionCount++;
                    if ((expectedOutput && transmission == "0") || (!expectedOutput && transmission == "1")) {
                        isValid = false;
                        break;
                    }
                    else {
                        expectedOutput = !expectedOutput;
                    }
                }
            }

            if (isValid) {
                return Constexpr::ToString(a - 1);
            }
        }

        throw std::logic_error("Wat?");
        */
    }
    PART_TWO() {
        return lines[0];
    }

    /*
    std::string RunTest(const std::vector<std::string>& lines, u64 a) {
        u32 instruction = 0;
        std::vector<u64> registers = { 0, 0, 0, 0 };
        registers[0] = a;
        std::string result;
        bool expectOne = false;
        while (result.size() < 50) {
            std::string out;
            ApplyInstruction(lines[instruction], instruction, registers, out);
            if (!out.empty()) {
                if ((expectOne && out == "0") || (!expectOne && out == "1")) {
                    break;
                }
                else {
                    result += out;
                    expectOne = !expectOne;
                }
            }
        }

        return result;
    }
    */
    TESTS() {
        /*
        static_assert(GetValue("a", { 12, 24 }) == 12);
        static_assert(GetValue("b", { 12, 24 }) == 24);
        static_assert(GetValue("4", { 12, 24 }) == 4);

        std::vector<std::string> oldLines = {
            "cpy a d",
            "cpy 9 c",
            "cpy 282 b",
            "inc d",
            "dec b",
            "jnz b -2",
            "dec c",
            "jnz c -5",
            "cpy d a",
            "jnz 0 0",
            "cpy a b",
            "cpy 0 a",
            "cpy 2 c",
            "jnz b 2",
            "jnz 1 6",
            "dec b",
            "dec c",
            "jnz c -4",
            "inc a",
            "jnz 1 -7",
            "cpy 2 b",
            "jnz c 2",
            "jnz 1 4",
            "dec b",
            "dec c",
            "jnz 1 -4",
            "jnz 0 0",
            "out b",
            "jnz a -19",
            "jnz 1 -21"
        };

        std::vector<std::string> newLines = {
            "cpy a d",
            "add d 2538",
            "cpy d a",
            "jnz 0 0",
            "div a 2 b",
            "out b",
            "jnz a -3",
            "jnz 1 -5"
        };

        for (u64 i = 1; i < 10000; i++) {
            if (RunTest(oldLines, i) != RunTest(newLines, i)) return false;
        }
        */
        return true;
    }
}