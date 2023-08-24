#include "2017/d10_KnotHash.h"
#include "2017/KnotHash.h"

SOLUTION(2017, 10) {
    PART_ONE() {
        auto numbers = KnotHash::GetInitialNumbers();

        u32 skip = 0;
        u32 currentPos = 0;

        auto lengths = ParseLineAsNumbers<u32>(Line);
        for (auto length : lengths) {
            KnotHash::Twist(numbers, length, skip, currentPos);
        }

        std::rotate(numbers.rbegin(), numbers.rbegin() + currentPos, numbers.rend());
        return Constexpr::ToString(numbers[0] * numbers[1]);
    }

    PART_TWO() {
        return KnotHash::HashToHex(KnotHash::Hash(Line));
    }

    TESTS() {
        //static_assert(PartTwo({ "1,2,3" }) == "3efbe78a8d82f29979031a4aa0b16a9d");

        return true;
    }
}