#include "2017/d15_Generators.h"

SOLUTION(2017, 15) {
    static constexpr u64 SeedA = 16807;
    static constexpr u64 SeedB = 48271;
    static constexpr u64 Divisor = 2147483647;
    static constexpr u64 Mask = 65535;
    static constexpr u64 InitialA = 722;
    static constexpr u64 InitialB = 354;

    constexpr u64 GenA(u64 previous) {
        return ((SeedA * previous) % Divisor);
    }
    constexpr u64 GenB(u64 previous) {
        return ((SeedB * previous) % Divisor);
    }

    static_assert((GenA(1181022009) & Mask) == (GenB(1233683848) & Mask));

    auto Part1() {
        u64 a = InitialA;
        u64 b = InitialB;
        u32 count = 0;
        for (auto i = 0; i < 40'000'000; i++) {
            a = GenA(a);
            b = GenB(b);
            if ((a & Mask) == (b & Mask)) count++;
        }
        return count;
    }

    auto Part2(u64 startA, u64 startB) {
        u64 a = startA;
        u64 b = startB;
        u32 count = 0;
        u32 comparisons = 0;
        while (comparisons < 5'000'000) {
            do {
                a = GenA(a);
            } while (a % 4 != 0);

            do {
                b = GenB(b);
            } while (b % 8 != 0);

            if ((a & Mask) == (b & Mask)) count++;
            comparisons++;
        }

        return count;
    }

    std::string Run(const std::vector<std::string>&) {
        //return Constexpr::ToString(Part1());
        return Constexpr::ToString(Part2(InitialA, InitialB));
    }

    bool RunTests() {

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