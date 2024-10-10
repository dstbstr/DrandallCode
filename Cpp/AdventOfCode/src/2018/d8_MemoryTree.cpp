#include "2018/d8_MemoryTree.h"

SOLUTION(2018, 8) {
    constexpr u32 SumMetadata(Constexpr::Queue<u32>& numbers) {
        auto children = numbers.front();
        numbers.pop();
        auto metadata = numbers.front();
        numbers.pop();

        u32 result = 0;

        for (u32 i = 0; i < children; i++) {
            result += SumMetadata(numbers);
        }
        for (u32 i = 0; i < metadata; i++) {
            result += numbers.front();
            numbers.pop();
        }

        return result;
    }

    constexpr u32 GetNodeValue(Constexpr::Queue<u32>& numbers) {
        auto children = numbers.front();
        numbers.pop();
        auto metadata = numbers.front();
        numbers.pop();

        u32 value = 0;
        if (children == 0) {
            for (u32 i = 0; i < metadata; i++) {
                value += numbers.front();
                numbers.pop();
            }
        }
        else {
            std::vector<u32> childValues;
            for (u32 i = 0; i < children; i++) {
                childValues.push_back(GetNodeValue(numbers));
            }
            std::vector<size_t> indexes;
            for (u32 i = 0; i < metadata; i++) {
                indexes.push_back(numbers.front());
                numbers.pop();
            }

            for (auto index : indexes) {
                if (index == 0 || childValues.size() < index) continue;
                value += childValues[index - 1];
            }
        }

        return value;
    }

    constexpr Constexpr::Queue<u32> GetNumbers(const auto& lines) {
        Constexpr::Queue<u32> numbers;
        auto split = Constexpr::Split(lines[0], " ");
        for (auto sv : split) {
            u32 num;
            Constexpr::ParseNumber(sv, num);
            numbers.push(num);
        }

        return numbers;
    }

    PART_ONE() {
        auto numbers = GetNumbers(lines);
        return Constexpr::ToString(SumMetadata(numbers));
    }

    PART_TWO() {
        auto numbers = GetNumbers(lines);
        return Constexpr::ToString(GetNodeValue(numbers));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2"
        };

        auto numbers = GetNumbers(lines);
        if (SumMetadata(numbers) != 138) return false;
        
        numbers = GetNumbers(lines);
        if (GetNodeValue(numbers) != 66) return false;

        return true;
    }
}