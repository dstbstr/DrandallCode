#include "2017/d10_KnotHash.h"
#include "2017/KnotHash.h"

SOLUTION(2017, 10) {
    auto Part1(const std::vector<u32>&lengths) {
        auto numbers = KnotHash::GetInitialNumbers();

        u32 skip = 0;
        u32 currentPos = 0;

        for (auto length : lengths) {
            KnotHash::Twist(numbers, length, skip, currentPos);
        }

        std::rotate(numbers.rbegin(), numbers.rbegin() + currentPos, numbers.rend());
        return numbers[0] * numbers[1];
    }

    auto Part2(const std::string & line) {
        return KnotHash::HashToHex(KnotHash::Hash(line));
    }

    std::string Run(const std::vector<std::string>&) {
        std::vector<u32> input = { 212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164 };

        //return Constexpr::ToString(Part1(input));
        return Part2("212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164");
    }

    bool RunTests() {
        if (Part2("1,2,3") != "3efbe78a8d82f29979031a4aa0b16a9d") return false;
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