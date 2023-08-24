#include "2017/d6_Realloc.h"
#include "Algorithms/FloydCycle.h"

SOLUTION(2017, 6) {
    constexpr std::vector<u32> Next(std::vector<u32> in) {
        auto max = std::max_element(in.begin(), in.end());
        for (u32 iters{ std::exchange(*max, 0) }; iters--; ++(*max)) {
            if (++max == in.end()) {
                max = in.begin();
            }
        }
        return in;
    }

    PART_ONE() {
        auto banks = ParseLineAsNumbers<u32>(Line, "\t");
        u32 cycleStart, cycleLength;
        FloydCycle::FindCycle(banks, Next, cycleLength, cycleStart);
        return Constexpr::ToString(cycleLength + cycleStart);
    }

    PART_TWO() {
        auto banks = ParseLineAsNumbers<u32>(Line, "\t");
        u32 cycleStart, cycleLength;
        FloydCycle::FindCycle(banks, Next, cycleLength, cycleStart);
        return Constexpr::ToString(cycleLength);
    }

    TESTS() {
        //static_assert(PartOne({ "0\t2\t7\t0" }) == "5");
        //static_assert(PartTwo({ "0\t2\t7\t0" }) == "4");
    
        return true;
    }
}