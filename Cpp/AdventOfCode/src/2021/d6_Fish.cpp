#include "2021/d6_Fish.h"

SOLUTION(2021, 6) {
    using Fish = std::array<size_t, 9>;

    constexpr Fish GetInitialFish(const std::string & line) {
        auto nums = ParseLineAsNumbers<size_t>(line);
        Fish result{};
        for (auto num : nums) {
            result[num]++;
        }
        return result;
    }

    constexpr void NextDay(Fish & fish) {
        Fish next{};
        next[8] = fish[0];
        for (size_t i = 1; i < 9; i++) {
            next[i - 1] = fish[i];
        }
        next[6] += next[8];
        fish = next;
    }

    constexpr bool TestNextDay() {
        Fish fish{ 0,1,1,2,1,0,0,0,0 };
        NextDay(fish);
        if (fish != Fish{ 1, 1, 2, 1, 0, 0, 0, 0, 0 }) return false;
        NextDay(fish);
        if (fish != Fish{ 1, 2, 1, 0, 0, 0, 1, 0, 1 }) return false;
        NextDay(fish);
        if (fish != Fish{ 2, 1, 0, 0, 0, 1, 1, 1, 1 }) return false;
        NextDay(fish);
        if (fish != Fish{ 1, 0, 0, 0, 1, 1, 3, 1, 2 }) return false;
        NextDay(fish);
        if (fish != Fish{ 0, 0, 0, 1, 1, 3, 2, 2, 1 }) return false;
        return true;
    }

    static_assert(TestNextDay());

    constexpr auto Solve(const std::string & line, size_t days) {
        auto fish = GetInitialFish(line);
        for (auto day = 0; day < days; day++) {
            NextDay(fish);
        }

        return std::accumulate(fish.begin(), fish.end(), 0ull);
    }
    auto Part1(const std::string & line) {
        return Solve(line, 80);
    }

    auto Part2(const std::string & line) {
        return Solve(line, 256);
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        if (!TestNextDay()) return false;
        std::string line = "3,4,3,1,2";
        if (Solve(line, 0) != 5) return false;
        if (Solve(line, 18) != 26) return false;
        if (Solve(line, 80) != 5934) return false;
        if (Solve(line, 256) != 26984457539) return false;
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