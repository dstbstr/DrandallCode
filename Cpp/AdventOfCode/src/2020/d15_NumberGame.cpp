#include "2020/d15_NumberGame.h"

SOLUTION(2020, 15) {
    template<size_t Rounds>
    constexpr auto Solve(const std::string & line) {
        constexpr const size_t sentinal = 999'999'999;
        auto numbers = ParseLineAsNumbers<size_t>(line);
        auto aPtr = std::make_unique<std::array<size_t, Rounds>>();
        auto& a = *aPtr;
        a.fill(sentinal);

        for (size_t i = 0; i < numbers.size() - 1; i++) {
            a[numbers[i]] = i;
        }

        size_t pos = numbers.size() - 1;
        size_t prev = numbers.back();
        size_t next = 0;
        while (pos <= Rounds) {
            next = 0;
            if (a[prev] != sentinal) {
                next = pos - a[prev];
            }
            a[prev] = pos;
            prev = next;
            ++pos;
        }

        return std::find(a.begin(), a.end(), Rounds - 1) - a.begin();
    }

    PART_ONE() {
        return Constexpr::ToString(Solve<2020>(lines[0]));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve<30'000'000>(lines[0]));
    }

    TESTS() {
        if (Solve<2020>("0,3,6") != 436) return false;
        if (Solve<2020>("0,3,6") != 436) return false;
        if (Solve<30'000'000>("0,3,6") != 175594) return false;

        return true;
    }
}