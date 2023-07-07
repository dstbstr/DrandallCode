#include "2017/d8_Registers.h"

SOLUTION(2017, 8) {
    using Regs = Constexpr::SmallMap<std::string, s32>;

    constexpr auto GenInstruction(const std::string & line) {
        auto s = Constexpr::Split(line, " ");
        auto condRegister = std::string(s[4]);
        auto op = std::string(s[5]);
        auto compareStr = s[6];
        
        s32 compareValue;
        Constexpr::ParseNumber(compareStr, compareValue);

        auto compFunc = [condRegister, op, compareValue](Regs& regs) {
            auto lhs = regs[condRegister];
            auto rhs = compareValue;

            if (op == "<") return lhs < rhs;
            else if (op == "<=") return lhs <= rhs;
            else if (op == "==") return lhs == rhs;
            else if (op == ">=") return lhs >= rhs;
            else if (op == ">") return lhs > rhs;
            else if (op == "!=") return lhs != rhs;
            throw "Unhandled operator";
        };

        auto targetRegister = std::string(s[0]);
        auto command = s[1];
        auto deltaStr = s[2];

        s32 delta;
        Constexpr::ParseNumber(deltaStr, delta);
        if (command == "dec") {
            delta = -delta;
        }

        return [targetRegister, delta, compFunc](Regs& regs) {
            if (compFunc(regs)) {
                regs[targetRegister] += delta;
            }
        };
    }

    constexpr s32 GetMaxElement(const Regs& regs) {
        auto values = regs.GetValues();
        return *std::max_element(values.begin(), values.end());
    }

    PART_ONE() {
        auto instructions = ParseLines(lines, GenInstruction);

        Regs regs;

        for (const auto& inst : instructions) {
            inst(regs);
        }

        return Constexpr::ToString(GetMaxElement(regs));
    }

    PART_TWO() {
        auto instructions = ParseLines(lines, GenInstruction);

        Regs regs;
        s32 max = 0;
        for (const auto& inst : instructions) {
            inst(regs);
            max = std::max(max, GetMaxElement(regs));
        }

        return Constexpr::ToString(max);
    }
    

    TESTS() {
        static_assert(PartOne({ "b inc 5 if a > 1", "a inc 1 if b < 5", "c dec -10 if a >= 1", "c inc -20 if c == 10" }) == "1");
        static_assert(PartTwo({ "b inc 5 if a > 1", "a inc 1 if b < 5", "c dec -10 if a >= 1", "c inc -20 if c == 10" }) == "10");

        return true;
    }
}