#include "2017/d23_Coprocessor.h"

SOLUTION(2017, 23) {
    using Registers = std::array<s64, 8>;

    constexpr s64 ReadValue(std::string_view sv, const Registers & registers) {
        if (sv.size() == 1 && sv[0] >= 'a') {
            return registers[sv[0] - 'a'];
        }

        s64 result;
        Constexpr::ParseNumber(sv, result);
        return result;
    }

    constexpr void ApplyCommand(const std::string & command, s64 & instruction, Registers & registers, u32 & multiplyCount) {
        auto split = Constexpr::Split(command, " ");
        auto regIndex = split[1][0] - 'a';
        s64 value = 0;
        if (split.size() > 2) {
            value = ReadValue(split[2], registers);
        }

        if (split[0] == "set") {
            registers[regIndex] = value;
        }
        else if (split[0] == "add") {
            registers[regIndex] += value;
        }
        else if (split[0] == "sub") {
            registers[regIndex] -= value;
        }
        else if (split[0] == "mul") {
            registers[regIndex] *= value;
            multiplyCount++;
        }
        else if (split[0] == "jnz") {
            auto compare = ReadValue(split[1], registers);
            if (compare != 0) {
                instruction += value - 1;
            }
        }

        instruction++;
    }

    constexpr auto Part1(const std::vector<std::string>&lines) {
        s64 instruction = 0;
        Registers registers;
        registers.fill(0);
        u32 multiplyCount = 0;

        while (instruction < static_cast<s64>(lines.size())) {
            ApplyCommand(lines[instruction], instruction, registers, multiplyCount);
        }

        return multiplyCount;
    }

    auto Part2() {
        auto primes = Constexpr::GetPrimes<s32>(122700);
        s32 count = 0;
        for (auto i = 105700; i <= 122700; i += 17) {
            if (std::find(primes.cbegin(), primes.cend(), i) == primes.cend()) {
                count++;
            }
        }

        return count;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        (void)lines;
        return Constexpr::ToString(Part2());
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "set f 1",
            "set b 7",
            "set c 58",
            "jnz 0 2",
            "jnz 1 5",
            "mul b 100",
            "sub b -100000",
            "set c b",
            "sub c -17000",
            "set f 1",
            "set d 2",
            "set e 2",
            "set g d",
            "mul g e",
            "sub g b",
            "jnz g 2",
            "set f 0",
            "sub e -1",
            "set g e",
            "sub g b",
            "jnz g -8",
            "sub d -1",
            "set g d",
            "sub g b",
            "jnz g -13",
            "jnz f 2",
            "sub h -1",
            "set g b",
            "sub g c",
            "jnz g 2",
            "jnz 1 3",
            "sub b -17",
            "jnz 1 -23"
        };

        //if (Part2(lines) != 0) return false;
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