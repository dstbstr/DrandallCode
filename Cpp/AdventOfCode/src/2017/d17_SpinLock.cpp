#include "2017/d17_SpinLock.h"

SOLUTION(2017, 17) {
    PART_ONE() {
        size_t input;
        Constexpr::ParseNumber(lines[0], input);
        size_t pos = 0;
        std::vector<size_t> buffer {0};
        for (auto i = 1; i <= 2017; i++) {
            pos = (pos + input) % i;
            buffer.insert(buffer.begin() + pos, i);
            pos++;
        }
        auto where = std::find(buffer.begin(), buffer.end(), 2017);
        return Constexpr::ToString(*(where + 1));
    }

    PART_TWO() {
        size_t input;
        Constexpr::ParseNumber(lines[0], input);
        size_t pos = 0;
        size_t result = 0;
        for (auto i = 1; i <= 50'000'000; i++) {
            pos = (pos + input) % i;
            if (pos++ == 0) {
                result = i;
            }
        }
        return Constexpr::ToString(result);
    }

    TESTS() {
        return true;
    }
}