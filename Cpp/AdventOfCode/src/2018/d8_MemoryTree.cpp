#include "2018/d8_MemoryTree.h"
#include <queue>

SOLUTION(2018, 8) {
    u32 SumMetadata(std::queue<u32>&numbers) {
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

    u32 GetNodeValue(std::queue<u32>&numbers) {
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

    std::queue<u32> GetNumbers(const std::vector<std::string>&lines) {
        std::queue<u32> numbers;
        auto split = Constexpr::Split(lines[0], " ");
        for (auto sv : split) {
            u32 num;
            Constexpr::ParseNumber(sv, num);
            numbers.push(num);
        }

        return numbers;
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto numbers = GetNumbers(lines);
        return SumMetadata(numbers);
    }

    auto Part2(const std::vector<std::string>&lines) {
        auto numbers = GetNumbers(lines);
        return GetNodeValue(numbers);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2"
        };

        if (Part1(lines) != 138) return false;
        if (Part2(lines) != 66) return false;
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