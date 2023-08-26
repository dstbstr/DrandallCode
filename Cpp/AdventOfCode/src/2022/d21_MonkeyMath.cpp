#include "2022/d21_MonkeyMath.h"

SOLUTION(2022, 21) {

    enum struct Op { Plus, Minus, Mul, Div };

    std::string ToString(Op op) {
        switch (op) {
        case Op::Plus: return "+";
        case Op::Minus: return "-";
        case Op::Mul: return "*";
        case Op::Div: return "/";
        default: return "";
        }
    }

    struct Monkey {
        std::string Name = "";
        u64 Value = 0;
        bool ValueSet = false;
        std::string Lhs = "";
        std::string Rhs = "";
        Op Operation = Op::Plus;
    };

    constexpr Monkey ParseLine(std::string_view line) {
        Monkey monkey{};
        monkey.Name = line.substr(0, 4);
        if (line.size() < 10) {
            Constexpr::ParseNumber(line.substr(6), monkey.Value);
            monkey.ValueSet = true;
        }
        else {
            monkey.Lhs = line.substr(6, 4);
            monkey.Rhs = line.substr(13);
            auto op = line[11];
            switch (op) {
            case '+': monkey.Operation = Op::Plus; break;
            case '-': monkey.Operation = Op::Minus; break;
            case '/': monkey.Operation = Op::Div; break;
            case '*': monkey.Operation = Op::Mul; break;
            }
        }

        return monkey;
    }

    constexpr u64 CalcValue(const Monkey & worker, const Monkey & lhs, const Monkey & rhs) {
        switch (worker.Operation) {
        case Op::Plus: return lhs.Value + rhs.Value;
        case Op::Minus: return lhs.Value - rhs.Value;
        case Op::Mul: return lhs.Value * rhs.Value;
        case Op::Div: return lhs.Value / rhs.Value;
        default: return 0;
        }
    }

    constexpr void RunMonkeys(std::vector<Monkey>&monkeys, const Constexpr::SmallMap<std::string, size_t>&monkeyMap, size_t rootIndex) {
        while (monkeys[rootIndex].ValueSet == false) {
            for (auto& monkey : monkeys) {
                if (monkey.ValueSet) continue;

                auto leftIndex = monkeyMap.at(monkey.Lhs);
                auto rightIndex = monkeyMap.at(monkey.Rhs);
                if (monkeys[leftIndex].ValueSet && monkeys[rightIndex].ValueSet) {
                    monkey.Value = CalcValue(monkey, monkeys[leftIndex], monkeys[rightIndex]);
                    monkey.ValueSet = true;
                }
            }
        }
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto monkeys = ParseLines(lines, ParseLine);
        size_t rootIndex = 0;

        Constexpr::SmallMap<std::string, size_t> monkeyMap;
        for (auto i = 0; i < monkeys.size(); i++) {
            monkeyMap[monkeys[i].Name] = i;
            if (monkeys[i].Name == "root") {
                rootIndex = i;
            }
        }

        RunMonkeys(monkeys, monkeyMap, rootIndex);
        return monkeys[rootIndex].Value;
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(Lines));
    }

    PART_TWO() {
        size_t rootIndex = 0;
        size_t humanIndex = 0;
        size_t leftMonkeyIndex = 0;
        size_t rightMonkeyIndex = 0;

        auto monkeys = ParseLines(Lines, ParseLine);

        Constexpr::SmallMap<std::string, size_t> monkeyMap;
        for (auto i = 0; i < monkeys.size(); i++) {
            monkeyMap[monkeys[i].Name] = i;
            if (monkeys[i].Name == "root") {
                rootIndex = i;
            }
            else if (monkeys[i].Name == "humn") {
                humanIndex = i;
            }
        }

        monkeys[humanIndex].ValueSet = false;
        monkeys[humanIndex].Value = 0;

        leftMonkeyIndex = monkeyMap[monkeys[rootIndex].Lhs];
        rightMonkeyIndex = monkeyMap[monkeys[rootIndex].Rhs];


        u32 added = 1;
        while (added > 0) {
            added = 0;
            for (auto& monkey : monkeys) {
                if (monkey.ValueSet || monkey.Name == "humn") continue;

                auto leftIndex = monkeyMap.at(monkey.Lhs);
                auto rightIndex = monkeyMap.at(monkey.Rhs);
                if (monkeys[leftIndex].ValueSet && monkeys[rightIndex].ValueSet) {
                    added++;
                    monkey.Value = CalcValue(monkey, monkeys[leftIndex], monkeys[rightIndex]);
                    monkey.ValueSet = true;
                }
            }
        }

        auto& currentMonkey = monkeys[rootIndex];
        currentMonkey.Operation = Op::Minus;

        while (true) {
            u64 targetValue = currentMonkey.Value;
            leftMonkeyIndex = monkeyMap[currentMonkey.Lhs];
            rightMonkeyIndex = monkeyMap[currentMonkey.Rhs];

            bool leftKnown = monkeys[leftMonkeyIndex].ValueSet;
            u64 knownValue = leftKnown ? monkeys[leftMonkeyIndex].Value : monkeys[rightMonkeyIndex].Value;
            auto& nextMonkey = leftKnown ? monkeys[rightMonkeyIndex] : monkeys[leftMonkeyIndex];

            u64 nextValue = 0;
            switch (currentMonkey.Operation) {
            case Op::Plus: nextValue = targetValue - knownValue; break;
            case Op::Mul: nextValue = targetValue / knownValue; break;
            }
            if (nextValue == 0) {
                if (leftKnown) {
                    switch (currentMonkey.Operation) {
                    case Op::Minus: nextValue = knownValue - targetValue; break;
                    case Op::Div: nextValue = knownValue / targetValue; break;
                    }
                }
                else {
                    switch (currentMonkey.Operation) {
                    case Op::Minus: nextValue = targetValue + knownValue; break;
                    case Op::Div: nextValue = targetValue * knownValue; break;
                    }
                }
            }

            nextMonkey.Value = nextValue;
            nextMonkey.ValueSet = true;
            if (nextMonkey.Name == "humn") {
                break;
            }
            currentMonkey = nextMonkey;
        }

        return Constexpr::ToString(monkeys[humanIndex].Value);
    }

    TESTS() {
        static_assert(ParseLine("root: pppw + sjmn").Name == "root");
        static_assert(ParseLine("root: pppw + sjmn").Operation == Op::Plus);
        static_assert(ParseLine("abcd: 42").Name == "abcd");
        static_assert(ParseLine("abcd: 42").Value == 42);

        std::vector<std::string> lines = {
            "root: pppw + sjmn",
            "dbpl: 5",
            "cczh: sllz + lgvd",
            "zczc: 2",
            "ptdq: humn - dvpt",
            "dvpt: 3",
            "lfqf: 4",
            "humn: 5",
            "ljgn: 2",
            "sjmn: drzm * dbpl",
            "sllz: 4",
            "pppw: cczh / lfqf",
            "lgvd: ljgn * ptdq",
            "drzm: hmdt - zczc",
            "hmdt: 32"
        };

        if (ParseLine("abcd: 42").Value != 42ull) return false;
        if (SolvePartOne(lines) != 152) return false;

        return true;
    }
}