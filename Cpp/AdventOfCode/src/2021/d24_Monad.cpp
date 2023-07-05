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

    size_t Solve(bool useMax) {
        std::unordered_map<size_t, size_t> seen;
        std::unordered_map<size_t, size_t> next;
        seen[0] = 0;

        for (size_t currentDigit = 0; currentDigit < 14; currentDigit++) {
            for (const auto& [remainder, value] : seen) {
                for (int i = 9; i > 0; i--) {
                    size_t result = remainder;
                    Eval(i, Entries[currentDigit], result);
                    size_t val = (value * 10) + i;
                    if (next.contains(result)) {
                        if (useMax) {
                            next[result] = std::max(val, next.at(result));
                        }
                        else {
                            next[result] = std::min(val, next.at(result));
                        }
                    }
                    else {
                        next[result] = val;
                    }
                }
            }

            seen = next;
            next.clear();
        }

        return seen[0];
    }
    auto Part1() {
        return Solve(true);
    }

    auto Part2() {
        return Solve(false);
    }

    std::string Run(const std::vector<std::string>&) {
        //return Constexpr::ToString(Part1());
        return Constexpr::ToString(Part2());
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