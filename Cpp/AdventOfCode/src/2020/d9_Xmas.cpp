#include "2020/d9_Xmas.h"

SOLUTION(2020, 9) {
    /* (4 * 5) / 2 == 10
1+2, 1+3, 1+4, 1+5
2+3, 2+4, 2+5
3+4, 3+5
4+5
*/
//(24 * 25) / 2 = 300

    template<typename T>
    constexpr std::vector<T> GetSums(std::vector<T> preamble) {
        std::vector<T> result;
        for (size_t i = 0; i < preamble.size(); i++) {
            for (size_t j = i + 1; j < preamble.size(); j++) {
                result.push_back(preamble[i] + preamble[j]);
            }
        }

        return result;
    }

    template<typename T>
    constexpr std::vector<T> ParseInput(const std::vector<std::string>&lines) {
        std::vector<size_t> nums;
        for (const auto& line : lines) {
            T num;
            Constexpr::ParseNumber(line, num);
            nums.push_back(num);
        }

        return nums;
    }

    auto FindKey(const auto & nums, size_t windowSize) {
        std::vector<size_t> window;
        for (auto i = 0; i < windowSize; i++) {
            window.push_back(nums[i]);
        }

        for (auto i = windowSize; i < nums.size(); i++) {
            auto sums = GetSums(window);
            if (std::find(sums.cbegin(), sums.cend(), nums[i]) == sums.cend()) {
                return nums[i];
            }

            std::rotate(window.begin(), window.begin() + 1, window.end());
            window.pop_back();
            window.push_back(nums[i]);
        }

        return 0ull;
    }

    auto Part1(const std::vector<std::string>&lines, size_t windowSize) {
        const auto nums = ParseInput<size_t>(lines);
        return FindKey(nums, windowSize);
    }

    template<typename T>
    constexpr void FindContiguousNums(const std::vector<T>&nums, T key, T & outMin, T & outMax) {
        std::vector<T> running;
        size_t next = 0;
        while (true) {
            //auto sum = Constexpr::Sum(running);
            auto sum = std::accumulate(running.begin(), running.end(), 0ull);
            if (sum < key) {
                running.push_back(nums[next++]);
            }
            else if (sum > key) {
                std::rotate(running.begin(), running.begin() + 1, running.end());
                running.pop_back();
            }
            else {
                outMin = Constexpr::FindMin(running);
                outMax = Constexpr::FindMax(running);
                if (outMin == outMax) continue;
                return;
            }
        }
    }

    auto Part2(const std::vector<std::string>&lines, size_t windowSize) {
        const auto nums = ParseInput<size_t>(lines);
        const auto key = FindKey(nums, windowSize);
        size_t min, max;
        FindContiguousNums(nums, key, min, max);

        return min + max;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 25));
        return Constexpr::ToString(Part2(lines, 25));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "35",
            "20",
            "15",
            "25",
            "47",
            "40",
            "62",
            "55",
            "65",
            "95",
            "102",
            "117",
            "150",
            "182",
            "127",
            "219",
            "299",
            "277",
            "309",
            "576"
        };

        if (Part1(lines, 5) != 127) return false;
        if (Part2(lines, 5) != 62) return false;
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