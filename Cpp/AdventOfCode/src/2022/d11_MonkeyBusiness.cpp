#include "2022/d11_MonkeyBusiness.h"

SOLUTION(2022, 11) {
    struct Monkey {
        std::vector<size_t> Items;
        size_t Divisor{ 0 };
        size_t TrueTarget{ 0 };
        size_t FalseTarget{ 0 };
        size_t ItemsHandled{ 0 };

        size_t Test(size_t val) {
            return val % Divisor ? FalseTarget : TrueTarget;
        }

        constexpr bool operator<(const Monkey& other) const {
            return other.ItemsHandled < ItemsHandled;
        }
    };

    enum struct Operation {Plus, Mul};
    constexpr auto ParseOperation(std::string_view line) {
        auto s = Constexpr::Split(line, " = ");
        auto s1 = Constexpr::Split(s[1], " ");
        size_t lhs, rhs;
        Constexpr::ParseNumber(s1[0], lhs);
        Constexpr::ParseNumber(s1[2], rhs);
        Operation op = (s1[1] == "*" ? Operation::Mul : Operation::Plus);

        return [lhs, rhs, op](size_t old) {
            size_t a = lhs;
            size_t b = rhs;
            if (lhs == 0) a = old;
            if (rhs == 0) b = old;
            switch (op) {
            case Operation::Plus: return a + b;
            case Operation::Mul: return a * b;
            default: throw "Unknown Operation";
            }
        };
    }

    constexpr std::vector<Monkey> ParseMonkeys(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        std::vector<Monkey> monkeys;
        for (const auto& group : groups) {
            auto s0 = Constexpr::Split(group[1], ": ");
            auto s1 = Constexpr::Split(s0[1], ", ");
            Monkey monkey;
            monkey.Items = ParseLinesAsNumbers<size_t>(s1);

            auto divisor = Constexpr::Split(group[3], " ").back();
            Constexpr::ParseNumber(divisor, monkey.Divisor);

            auto trueTarget = Constexpr::Split(group[4], " ").back();
            auto falseTarget = Constexpr::Split(group[5], " ").back();
            Constexpr::ParseNumber(trueTarget, monkey.TrueTarget);
            Constexpr::ParseNumber(falseTarget, monkey.FalseTarget);

            monkeys.push_back(monkey);
        }

        return monkeys;
    }

    constexpr std::vector<std::string> GetOpLines(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        std::vector<std::string> result;
        for (const auto& group : groups) {
            result.push_back(group[2]);
        }
        return result;
    }

    constexpr size_t Solve(const auto& lines, size_t rounds, size_t divisor) {
        auto monkeys = ParseMonkeys(lines);
        auto ops = ParseLines(GetOpLines(lines), ParseOperation);

        auto mod = std::accumulate(monkeys.begin(), monkeys.end(), 1ull, [](size_t running, const Monkey& monkey) {
            return running * monkey.Divisor;
            });

        u64 running = 0;
        for (auto round = 0; round < rounds; round++) {
            for (size_t i = 0; i < monkeys.size(); i++) {
                auto& monkey = monkeys[i];
                monkey.ItemsHandled += monkey.Items.size();
                for (auto item : monkey.Items) {
                    running = ops[i](item% mod) / divisor;
                    monkeys[monkey.Test(running)].Items.push_back(running);
                }
                monkey.Items.clear();
            }
        }
        std::sort(monkeys.begin(), monkeys.end());
        return monkeys[0].ItemsHandled * monkeys[1].ItemsHandled;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Lines, 20, 3));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Lines, 10'000, 1));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "Monkey 0:",
            "  Starting items: 79, 98",
            "  Operation: new = old * 19",
            "  Test: divisible by 23",
            "    If true: throw to monkey 2",
            "    If false: throw to monkey 3",
            "",
            "Monkey 1:",
            "  Starting items: 54, 65, 75, 74",
            "  Operation: new = old + 6",
            "  Test: divisible by 19",
            "    If true: throw to monkey 2",
            "    If false: throw to monkey 0",
            "",
            "Monkey 2:",
            "  Starting items: 79, 60, 97",
            "  Operation: new = old * old",
            "  Test: divisible by 13",
            "    If true: throw to monkey 1",
            "    If false: throw to monkey 3",
            "",
            "Monkey 3:",
            "  Starting items: 74",
            "  Operation: new = old + 3",
            "  Test: divisible by 17",
            "    If true: throw to monkey 0",
            "    If false: throw to monkey 1"
        };

        if (Solve(lines, 20, 3) != 10605) return false;
        if (Solve(lines, 10'000, 1) != 2713310158) return false;
        return true;
    }
}