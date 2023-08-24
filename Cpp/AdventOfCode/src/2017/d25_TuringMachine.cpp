#include "2017/d25_TuringMachine.h"

SOLUTION(2017, 25) {

    /*
    Begin in state A.
Perform a diagnostic checksum after 12667664 steps.
    */
    constexpr char GetStartState(std::string_view startStateLine) {
        return startStateLine.substr(startStateLine.size() - 2)[0];
    }
    constexpr size_t GetSteps(const std::string& stepsLine) {
        auto s = Constexpr::Split(stepsLine, " ");
        size_t result;
        Constexpr::ParseNumber(s[5], result);
        return result;
    }

    template<typename TrueFunc, typename FalseFunc>
    struct Rule {
        FalseFunc OnFalse;
        TrueFunc OnTrue;

        void operator()(std::vector<u8>& bits, size_t& pos, char& state) {
            bits[pos] == 1 ? OnTrue(bits, pos, state) : OnFalse(bits, pos, state);
        }
    };

    /*
In state A:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state B.
  If the current value is 1:
    - Write the value 0.
    - Move one slot to the left.
    - Continue with state C.

    */

    constexpr auto GetRule(const std::string& l1, const std::string& l2, const std::string& l3) {
        u8 toWrite = l1.substr(l1.size() - 2)[0] == '1' ? 1 : 0;
        char nextState = l3.substr(l3.size() - 2)[0] - 'A';
        auto s = Constexpr::Split(l2, " ");
        s8 delta = s.back() == "right." ? 1 : -1;

        return [=](std::vector<u8>& bits, size_t& pos, char& state) {
            bits[pos] = toWrite;
            pos += delta;
            state = nextState;
        };
    }

    constexpr auto ParseRule(const std::vector<std::string> lines) {
        return Rule{
            GetRule(lines[2], lines[3], lines[4]),
            GetRule(lines[6], lines[7], lines[8])
        };
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(Lines);
        char state = GetStartState(groups[0][0]) - 'A';

        auto steps = GetSteps(groups[0][1]);
        std::vector<u8> bits(1'000'000, 0);

        size_t pos = 500'000;
        using RuleType = decltype(ParseRule({ "" }));
        std::vector<RuleType> rules;
        for (size_t i = 1; i < groups.size(); i++) {
            rules.push_back(ParseRule(groups[i]));
        }

        for (size_t step = 0; step < steps; step++) {
            rules[state](bits, pos, state);
        }

        return Constexpr::ToString(std::count(bits.begin(), bits.end(), 1));
    }

    PART_TWO() {
        return "Merry Christmas";
    }

    TESTS() {
        //std::vector<std::string> lines = {
        //    "Begin in state A.",
        //    "Perform a diagnostic checksum after 6 steps.",
        //    "",
        //    "In state A:",
        //    "  If the current value is 0:",
        //    "    - Write the value 1.",
        //    "    - Move one slot to the right.",
        //    "    - Continue with state B.",
        //    "  If the current value is 1:",
        //    "    - Write the value 0.",
        //    "    - Move one slot to the left.",
        //    "    - Continue with state B.",
        //    "",
        //    "In state B:",
        //    "  If the current value is 0:",
        //    "    - Write the value 1.",
        //    "    - Move one slot to the left.",
        //    "    - Continue with state A.",
        //    "  If the current value is 1:",
        //    "    - Write the value 1.",
        //    "    - Move one slot to the right.",
        //    "    - Continue with state A."
        //};
        //if (PartOne(lines) != "3") return false;

        return true;
    }
}