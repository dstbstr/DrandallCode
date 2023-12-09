#include "2017/d15_Generators.h"

SOLUTION(2017, 15) {
    static constexpr u64 SeedA = 16807;
    static constexpr u64 SeedB = 48271;
    static constexpr u64 Divisor = 2147483647;
    static constexpr u64 Mask = 65535;

    constexpr u64 GenA(u64 previous) {
        return ((SeedA * previous) % Divisor);
    }
    constexpr u64 GenB(u64 previous) {
        return ((SeedB * previous) % Divisor);
    }

    static_assert((GenA(1181022009) & Mask) == (GenB(1233683848) & Mask));

    constexpr void ParseLines(const auto& lines, size_t& a, size_t& b) {
        auto s1 = Constexpr::Split(lines[0], " ");
        auto s2 = Constexpr::Split(lines[1], " ");
        Constexpr::ParseNumber(s1.back(), a);
        Constexpr::ParseNumber(s2.back(), b);
    }
    
    //This version is about twice as fast
    constexpr size_t gen(size_t g, size_t f) {
        size_t prod = g * f;
        g = (prod & Divisor) + (prod >> 31);
        return g >> 31 ? g - Divisor : g;
    }
    constexpr size_t solve(size_t a, size_t ma, size_t b, size_t mb, size_t r) {
        size_t result = 0;
        while (r--) {
            do a = gen(a, SeedA); while (a & ma);
            do b = gen(b, SeedB); while (b & mb);
            result += ((a ^ b) & Mask) == 0;
        }
        return result;
    }

    PART_ONE() {
        u64 a, b;
        ParseLines(lines, a, b);
        return Constexpr::ToString(solve(a, 0, b, 0, 40'000'000ull));
    }

    PART_TWO() {
        u64 a, b;
        ParseLines(lines, a, b);
        return Constexpr::ToString(solve(a, 3, b, 7, 5'000'000ull));
    }
    
    /*
    PART_ONE() {
        u64 a, b;
        ParseLines(lines, a, b);

        u32 count = 0;
        for (auto i = 0; i < 40'000'000; i++) {
            a = GenA(a);
            b = GenB(b);
            if ((a & Mask) == (b & Mask)) count++;
        }
        return Constexpr::ToString(count);
    }

    PART_TWO() {
        u64 a, b;
        ParseLines(lines, a, b);

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

        return Constexpr::ToString(count);
    }
    */
    TESTS() {
        return true;
    }
}