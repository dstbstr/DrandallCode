#pragma once

#include "2015/d17_EggnogStorage.h"

SOLUTION(2015, 17) {
    constexpr u32 CountCombinations(const std::vector<u32>&containers, size_t index, u32 remaining) {
        auto next = containers[index];

        if (remaining < next) {
            return 0;
        }

        remaining -= next;
        if (remaining == 0) {
            return 1;
        }

        u32 result = 0;
        for (auto i = index + 1; i < containers.size(); i++) {
            result += CountCombinations(containers, i, remaining);
        }

        return result;
    }

    constexpr u32 CountCombinations(std::vector<u32>&containerSizes, u32 initial) {
        containerSizes.push_back(0); // :(
        std::sort(containerSizes.begin(), containerSizes.end());
        return CountCombinations(containerSizes, 0, initial);
    }

    constexpr u32 GetMinSize(const std::vector<u32>&containers, size_t index, u32 remaining, u32 currentLength) {
        auto next = containers[index];
        if (remaining < next) {
            return 0;
        }
        remaining -= next;
        if (remaining == 0) {
            return currentLength + 1;
        }

        u32 best = 999;
        for (auto i = index + 1; i < containers.size(); i++) {
            u32 length = GetMinSize(containers, i, remaining, currentLength + 1);
            if (length > 0) {
                best = std::min(best, length);
            }
        }

        return best;
    }

    constexpr u32 CountCombinationsWithMaxContainers(const std::vector<u32>&containers, size_t index, u32 remaining, u32 currentLength, const u32 maxSize) {
        if (currentLength >= maxSize) {
            return 0;
        }

        auto next = containers[index];
        if (remaining < next) {
            return 0;
        }
        remaining -= next;
        if (remaining == 0) {
            return 1;
        }

        u32 result = 0;
        for (auto i = index + 1; i < containers.size(); i++) {
            result += CountCombinationsWithMaxContainers(containers, i, remaining, currentLength + 1, maxSize);
        }

        return result;
    }

    constexpr u32 CountSmallContainers(std::vector<u32>&containers, u32 initial) {
        containers.push_back(0); // :(
        std::sort(containers.begin(), containers.end());

        u32 size = GetMinSize(containers, 0, initial, 0);

        return CountCombinationsWithMaxContainers(containers, 0, initial, 0, size);
    }

    PART_ONE() {
        auto containers = ParseLinesAsNumbers<u32>(Lines);
        return Constexpr::ToString(CountCombinations(containers, 150));
    }
    PART_TWO() {
        auto containers = ParseLinesAsNumbers<u32>(Lines);
        return Constexpr::ToString(CountSmallContainers(containers, 150));
    }

    TESTS() {
        std::vector<u32> containers = {20, 5, 10, 5, 15};
        std::sort(containers.begin(), containers.end());

        if (CountCombinations(containers, 25) != 4) return false;
        if(GetMinSize(containers, 0, 25, 0) != 3) return false; //0 10 15
        if(CountSmallContainers(containers, 25) != 3) return false;

        return true;
    }
}