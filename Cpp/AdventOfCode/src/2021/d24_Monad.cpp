#include "2021/d24_Monad.h"

SOLUTION(2021, 24) {
    struct Entry {
        int MatchDelta;
        int SumDelta;
        bool Shrink;
    };

    constexpr std::array<Entry, 14> Entries = { {
        { 11,  6, 0},
        { 11, 12, 0},
        { 15,  8, 0},
        {-11,  7, 1},
        { 15,  7, 0},
        { 15, 12, 0},
        { 14,  2, 0},
        { -7, 15, 1},
        { 12,  4, 0},
        { -6,  5, 1},
        {-10, 12, 1},
        {-15, 11, 1},
        { -9, 13, 1},
        {  0,  7, 1}
    } };

    constexpr void Eval(int input, const Entry & e, size_t & result) {
        bool match = (result % 26) + e.MatchDelta != input;
        result /= (25 * e.Shrink) + 1;

        int mul = (25 * match) + 1;
        int add = (input + e.SumDelta) * match;

        result = (result * mul) + add;
    }

    constexpr bool Recurse(size_t z, size_t digit, size_t acc, bool useMax, size_t& outResult) {
        if (digit == Entries.size()) {
            if (z == 0) {
                outResult = acc;
                return true;
            }
            return false;
        }

        for (auto i = useMax ? 9 : 1; (useMax && i > 0) || (!useMax && i < 10); useMax ? i-- : i++) {
            auto entry = Entries[digit];
            if (entry.MatchDelta > 0 || z % 26 == i - entry.MatchDelta) {
                auto newZ = z;
                Eval(i, entry, newZ);
                size_t result;
                if (Recurse(newZ, digit + 1, acc * 10 + i, useMax, result)) {
                    outResult = result;
                    return true;
                }
            }
        }

        return false;
    }
    constexpr size_t Solve(bool useMax) {
        size_t result;
        Recurse(0, 0, 0, useMax, result);
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(true));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(false));
    }

    TESTS() {
        return true;
    }
}