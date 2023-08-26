#include "2022/d20_Mixing.h"

SOLUTION(2022, 20) {
    constexpr void Shift(std::vector<s64>&numbers, size_t index, std::vector<size_t>&indexes) {
        const s64 len = numbers.size() - 1;
        s64 val = numbers[index];
        size_t targetIndex = Constexpr::EuclideanModulo(static_cast<s64>(index) + val, len);

        numbers.erase(numbers.begin() + index);
        numbers.insert(numbers.begin() + targetIndex, val);

        auto srcIndex = indexes[index];
        indexes.erase(indexes.begin() + index);
        indexes.insert(indexes.begin() + targetIndex, srcIndex);

        if (targetIndex == 0) {
            std::rotate(numbers.begin(), numbers.begin() + 1, numbers.end());
            std::rotate(indexes.begin(), indexes.begin() + 1, indexes.end());
        }
    }

    constexpr size_t FindIndexWithValue(auto targetValue, const auto& indexes) {
        for (size_t i = 0; i < indexes.size(); i++) {
            if (indexes[i] == targetValue) {
                return i;
            }
        }

        return 0;
    }

    constexpr std::vector<s64> Mix(const std::vector<s64>&numbers, std::vector<size_t>&indexes) {
        std::vector<s64> result = numbers;
        for (size_t index = 0; index < numbers.size(); index++) {
            auto resultIndex = FindIndexWithValue(index, indexes);
            Shift(result, resultIndex, indexes);
        }

        return result;
    }

    constexpr s64 Solve(const auto& lines, s64 key, s64 count) {
        auto numbers = ParseLinesAsNumbers<s64>(lines);
        for (auto& number : numbers) {
            number *= key;
        }

        std::vector<size_t> indexes;
        for (size_t index = 0; index < numbers.size(); index++) {
            indexes.push_back(index);
        }

        for (auto i = 0; i < count; i++) {
            numbers = Mix(numbers, indexes);
        }

        auto zeroIndex = FindIndexWithValue(0, numbers);

        auto x = (zeroIndex + 1000) % numbers.size();
        auto y = (zeroIndex + 2000) % numbers.size();
        auto z = (zeroIndex + 3000) % numbers.size();

        return numbers[x] + numbers[y] + numbers[z];
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(Lines, 1, 1));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(Lines, 811589153, 10));
    }

    constexpr bool TestShift(size_t index, std::vector<s64> start, std::vector<s64> end) {
        std::vector<size_t> indexes;
        for (auto i = 0; i < start.size(); i++) {
            indexes.push_back(i);
        }

        Shift(start, index, indexes);
        return start == end;
    }

    TESTS() {
        static_assert(TestShift(0, { 1, 2, -3, 3, -2, 0, 4 }, { 2, 1, -3, 3, -2, 0, 4 }));
        static_assert(TestShift(0, { 2, 1, -3, 3, -2, 0, 4 }, { 1, -3, 2, 3, -2, 0, 4 }));
        static_assert(TestShift(1, { 1, -3, 2, 3, -2, 0, 4 }, { 1, 2, 3, -2, -3, 0, 4 }));
        static_assert(TestShift(2, { 1, 2, 3, -2, -3, 0, 4 }, { 1, 2, -2, -3, 0, 3, 4 }));
        static_assert(TestShift(2, { 1, 2, -2, -3, 0, 3, 4 }, { 1, 2, -3, 0, 3, 4, -2 }));
        static_assert(TestShift(3, { 1, 2, -3, 0, 3, 4, -2 }, { 1, 2, -3, 0, 3, 4, -2 }));
        static_assert(TestShift(5, { 1, 2, -3, 0, 3, 4, -2 }, { 1, 2, -3, 4, 0, 3, -2 }));

        std::vector<std::string> lines = {"1","2","-3","3","-2","0","4"};

        if (Solve(lines, 1, 1) != 3) return false;
        if (Solve(lines, 811589153, 10) != 1623178306) return false;

        return true;
    }
}