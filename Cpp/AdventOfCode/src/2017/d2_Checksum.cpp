#include "2017/d2_Checksum.h"
#include <execution>

SOLUTION(2017, 2) {
    constexpr u32 FindDelta(std::string_view line) {
        auto values = ParseLineAsNumbers<u32>(line, "\t");
        std::sort(values.begin(), values.end());
        return values.back() - values[0];
    }

    constexpr u32 FindDivisors(std::string_view line) {
        auto values = ParseLineAsNumbers<u32>(line, "\t");
        std::sort(values.begin(), values.end());
        for (size_t i = 0; i < values.size(); i++) {
            for (size_t j = i + 1; j < values.size(); j++) {
                if (values[j] % values[i] == 0) {
                    return values[j] / values[i];
                }
            }
        }
        return 0;
    }

    constexpr u32 Checksum(const auto& lines, auto func) {
        return std::transform_reduce(lines.cbegin(), lines.cend(), 0u, std::plus<u32>(), func);
    }

    PART_ONE() {
        return Constexpr::ToString(Checksum(lines, FindDelta));
    }

    PART_TWO() {
        return Constexpr::ToString(Checksum(lines, FindDivisors));
    }

    TESTS() {
        static_assert(FindDelta("5\t1\t9\t5") == 8);
        static_assert(FindDelta("7\t5\t3") == 4);
        static_assert(FindDelta("2\t4\t6\t8") == 6);
        
        static_assert(Checksum(std::vector<std::string>{"5\t1\t9\t5", "7\t5\t3", "2\t4\t6\t8"}, FindDelta) == 18);
        static_assert(Checksum(std::vector<std::string>{"5\t9\t2\t8", "9\t4\t7\t3", "3\t8\t6\t5"}, FindDivisors) == 9);

        return true;
    }
}