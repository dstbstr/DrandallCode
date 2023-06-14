#include "2015/d1_SantaElevator.h"

SOLUTION(2015, 1) {
    constexpr size_t FindFloor(const std::string_view input) {
        return std::count(input.cbegin(), input.cend(), '(') - std::count(input.cbegin(), input.cend(), ')');
    }

    constexpr size_t FindBasementStep(const std::string_view input) {
        s32 currentFloor = 0;
        for (size_t step = 0; step < input.size(); step++) {
            currentFloor += (input[step] == '(' ? 1 : -1);
            if (currentFloor < 0) return step + 1;
        }
        return 0;
    }

    PART_ONE() {
        return Constexpr::ToString(FindFloor(lines[0]));
    }
    PART_TWO() {
        return Constexpr::ToString(FindBasementStep(lines[0]));
    }

    TESTS() {
        static_assert(FindFloor("") == 0);
        static_assert(FindFloor("(())") == 0);
        static_assert(FindFloor("()()") == 0);
        static_assert(FindFloor("(((") == 3);
        static_assert(FindFloor("(()(()(") == 3);
        static_assert(FindFloor("())") == -1);
        static_assert(FindFloor("))(") == -1);
        static_assert(FindFloor(")))") == -3);
        static_assert(FindFloor(")())())") == -3);

        static_assert(FindBasementStep(")") == 1);
        static_assert(FindBasementStep("()())") == 5);

        return true;
    }
}