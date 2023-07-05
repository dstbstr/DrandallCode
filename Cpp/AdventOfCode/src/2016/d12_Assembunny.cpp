#include "2016/d12_Assembunny.h"

SOLUTION(2016, 12) {
    constexpr s32 GetValue(std::string_view sv, const std::vector<s32>&registers) {
        if (sv[0] <= 'd' && sv[0] >= 'a') {
            return registers[sv[0] - 'a'];
        }
        else {
            s32 result;
            Constexpr::ParseNumber(sv, result);
            return result;
        }
    }

    constexpr auto GenInstruction(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        auto cmd = std::string(s[0]);
        auto arg1 = std::string(s[1]);
        std::string arg2 = "";
        if (s.size() > 2) {
            arg2 = std::string(s[2]);
        }

        return [cmd, arg1, arg2](size_t& ip, std::vector<s32>& regs) {
            if (cmd == "cpy") {
                auto value = GetValue(arg1, regs);
                regs[arg2[0] - 'a'] = value;
            }
            else if (cmd == "inc") {
                regs[arg1[0] - 'a']++;
            }
            else if (cmd == "dec") {
                regs[arg1[0] - 'a']--;
            }
            else if (cmd == "jnz") {
                if (GetValue(arg1, regs) != 0) {
                    ip += GetValue(arg2, regs);
                    return;
                }
            }
            ip++;
        };
    }

    constexpr void Execute(const std::vector<std::string>& lines, std::vector<s32>& regs) {
        using Inst = decltype(GenInstruction(""));
        auto instructions = ParseLines<Inst>(lines, GenInstruction);
        size_t ip = 0;

        while (ip < instructions.size()) {
            instructions[ip](ip, regs);
        }
    }
    PART_ONE() {
        std::vector<s32> registers{ 0, 0, 0, 0 };
        Execute(lines, registers);

        return Constexpr::ToString(registers[0]);
    }
    PART_TWO() {
        std::vector<s32> registers{ 0, 0, 1, 0 };
        Execute(lines, registers);

        return Constexpr::ToString(registers[0]);
    }

    TESTS() {
        static_assert(GetValue("a", { 12, 24 }) == 12);
        static_assert(GetValue("b", { 12, 24 }) == 24);
        static_assert(GetValue("4", { 12, 24 }) == 4);

        std::vector<std::string> lines = {
            "cpy 41 a",
            "inc a",
            "inc a",
            "dec a",
            "jnz a 2",
            "dec a"
        };

        std::vector<s32> registers{ 0, 0, 0, 0 };
        Execute(lines, registers);
        if (registers[0] != 42) return false;

        return true;
    }
}

