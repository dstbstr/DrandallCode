#include "2017/d6_Realloc.h"

SOLUTION(2017, 6) {

    // 0 2 7 0
    // 7 / 4 = 1
    // 7 % 4 = 3
    // all banks get 1
    // 3 banks get 2

    constexpr void Distribute(std::vector<u32>&banks, size_t bankIndex) {
        auto min = static_cast<u32>(banks[bankIndex] / banks.size());
        auto extras = static_cast<u32>(banks[bankIndex] % banks.size());

        banks[bankIndex] = 0;
        for (auto& bank : banks) {
            bank += min;
        }

        for (u32 i = 0; i < extras; i++) {
            banks[(bankIndex + i + 1) % banks.size()]++;
        }
    }

    constexpr bool TestDistribute(const std::vector<u32>&initial, u32 bank, const std::vector<u32>&expected) {
        std::vector<u32> actual = initial;
        Distribute(actual, bank);
        return actual == expected;
    }

    static_assert(TestDistribute({ 0, 2, 7, 0 }, 2, { 2, 4, 1, 2 }));

    constexpr size_t FindTargetBank(const std::vector<u32> banks) {
        u32 max = 0;
        size_t result = 0;
        for (size_t i = 0; i < banks.size(); i++) {
            if (banks[i] > max) {
                max = banks[i];
                result = i;
            }
        }

        return result;
    }

    static_assert(FindTargetBank({ 0, 2, 7, 0 }) == 2);

    auto Part1(const std::string & line) {
        std::unordered_set<std::string> seen;
        std::vector<u32> banks;
        auto split = Constexpr::Split(line, "\t");
        for (auto sv : split) {
            u32 num;
            Constexpr::ParseNumber(sv, num);
            banks.push_back(num);
        }

        while (true) {
            auto asString = StrUtil::JoinVec(',', banks);
            if (seen.find(asString) != seen.end()) break;
            seen.insert(asString);

            Distribute(banks, FindTargetBank(banks));
        }
        return seen.size();
    }

    auto Part2(const std::string & line) {
        std::vector<std::string> seen;
        std::vector<u32> banks;
        auto split = Constexpr::Split(line, "\t");
        for (auto sv : split) {
            u32 num;
            Constexpr::ParseNumber(sv, num);
            banks.push_back(num);
        }

        while (true) {
            auto asString = StrUtil::JoinVec(',', banks);
            for (auto i = 0; i < seen.size(); i++) {
                if (seen[i] == asString) {
                    return seen.size() - i;
                }
            }
            seen.push_back(asString);

            Distribute(banks, FindTargetBank(banks));
        }

        throw std::logic_error("Wat?");
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        if (Part1("0\t2\t7\t0") != 5) return false;
        if (Part2("0\t2\t7\t0") != 4) return false;
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