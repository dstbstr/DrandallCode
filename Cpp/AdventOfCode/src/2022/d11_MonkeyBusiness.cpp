#include "2022/d11_MonkeyBusiness.h"

SOLUTION(2022, 11) {
    struct Monkey {
        std::vector<u64> Items;
        std::function<u64(u64)> Operation;
        std::function<u64(u64)> Test;

        u64 Modulus = 1;
        u64 ItemsHandled = 0;
    };

    std::vector<Monkey> InitMonkeys() {
        Monkey m0;
        m0.Items = { 85, 77, 77 };
        m0.Operation = [](u64 input) { return input * 7; };
        m0.Test = [](u64 input) { return input % 19 == 0 ? 6 : 7; };
        m0.Modulus = 19;

        Monkey m1;
        m1.Items = { 80, 99 };
        m1.Operation = [](u64 input) { return input * 11; };
        m1.Test = [](u64 input) { return input % 3 == 0 ? 3 : 5; };
        m1.Modulus = 3;

        Monkey m2;
        m2.Items = { 74, 60, 74, 63, 86, 92, 80 };
        m2.Operation = [](u64 input) { return input + 8; };
        m2.Test = [](u64 input) { return input % 13 == 0 ? 0 : 6; };
        m2.Modulus = 13;

        Monkey m3;
        m3.Items = { 71, 58, 93, 65, 80, 68, 54, 71 };
        m3.Operation = [](u64 input) { return input + 7; };
        m3.Test = [](u64 input) { return input % 7 == 0 ? 2 : 4; };
        m3.Modulus = 7;

        Monkey m4;
        m4.Items = { 97, 56, 79, 65, 58 };
        m4.Operation = [](u64 input) { return input + 5; };
        m4.Test = [](u64 input) { return input % 5 == 0 ? 2 : 0; };
        m4.Modulus = 5;

        Monkey m5;
        m5.Items = { 77 };
        m5.Operation = [](u64 input) { return input + 4; };
        m5.Test = [](u64 input) {return input % 11 == 0 ? 4 : 3; };
        m5.Modulus = 11;

        Monkey m6;
        m6.Items = { 99, 90, 84, 50 };
        m6.Operation = [](u64 input) { return input * input; };
        m6.Test = [](u64 input) {return input % 17 == 0 ? 7 : 1; };
        m6.Modulus = 17;

        Monkey m7;
        m7.Items = { 50, 66, 61, 92, 64, 78 };
        m7.Operation = [](u64 input) { return input + 3; };
        m7.Test = [](u64 input) {return input % 2 == 0 ? 5 : 1; };
        m7.Modulus = 2;

        return { m0, m1, m2, m3, m4, m5, m6, m7 };
    }

    void RunRounds(std::vector<Monkey>&monkeys) {
        for (auto round = 0; round < 20; round++) {
            for (auto& monkey : monkeys) {
                monkey.ItemsHandled += static_cast<u64>(monkey.Items.size());
                for (auto item : monkey.Items) {
                    u64 running = monkey.Operation(item);
                    running /= 3;
                    u64 next = monkey.Test(running);
                    monkeys[next].Items.push_back(running);
                }
                monkey.Items.clear();
            }
        }
    }

    void RunHardRounds(std::vector<Monkey>&monkeys) {
        u64 mod = 1;
        for (const auto& monkey : monkeys) {
            mod *= monkey.Modulus;
        }

        for (auto round = 0; round < 10000; round++) {
            for (auto& monkey : monkeys) {
                monkey.ItemsHandled += static_cast<u64>(monkey.Items.size());
                for (auto item : monkey.Items) {
                    u64 running = item % mod;
                    running = monkey.Operation(running);
                    u64 next = monkey.Test(running);
                    monkeys[next].Items.push_back(running);
                }
                monkey.Items.clear();
            }
        }
    }
    std::string Run(const std::vector<std::string>&) {
        auto monkeys = InitMonkeys();
        //RunRounds(monkeys);
        RunHardRounds(monkeys);

        std::sort(monkeys.begin(), monkeys.end(), [](Monkey& lhs, Monkey& rhs) { return lhs.ItemsHandled > rhs.ItemsHandled; });

        return Constexpr::ToString(monkeys[0].ItemsHandled * monkeys[1].ItemsHandled);
    }

    std::vector<Monkey> InitTestMonkeys() {
        Monkey m0;
        m0.Items = { 79, 98 };
        m0.Operation = [](u64 input) { return input * 19; };
        m0.Test = [](u64 input) { return input % 23 == 0 ? 2 : 3; };
        m0.Modulus = 23;

        Monkey m1;
        m1.Items = { 54, 65, 75, 74 };
        m1.Operation = [](u64 input) { return input + 6; };
        m1.Test = [](u64 input) { return input % 19 == 0 ? 2 : 0; };
        m1.Modulus = 19;

        Monkey m2;
        m2.Items = { 79, 60, 97 };
        m2.Operation = [](u64 input) { return input * input; };
        m2.Test = [](u64 input) { return input % 13 == 0 ? 1 : 3; };
        m2.Modulus = 13;

        Monkey m3;
        m3.Items = { 74 };
        m3.Operation = [](u64 input) { return input + 3; };
        m3.Test = [](u64 input) { return input % 17 == 0 ? 0 : 1; };
        m3.Modulus = 17;

        return { m0, m1, m2, m3 };
    }

    bool RunTests() {
        auto monkeys = InitTestMonkeys();
        RunHardRounds(monkeys);

        if (monkeys[0].ItemsHandled != 52166) return false;
        if (monkeys[1].ItemsHandled != 47830) return false;
        if (monkeys[2].ItemsHandled != 1938) return false;
        if (monkeys[3].ItemsHandled != 52013) return false;

        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}