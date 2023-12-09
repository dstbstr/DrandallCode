#include "2015/d1_SantaElevator.h"

SOLUTION(2015, 1) {
    constexpr size_t FindFloor(const std::string_view val) {
        return std::count(val.cbegin(), val.cend(), '(') - std::count(val.cbegin(), val.cend(), ')');
    }

    constexpr size_t FindBasementStep(const std::string_view val) {
        s32 currentFloor = 0;
        for (size_t step = 0; step < val.size(); step++) {
            currentFloor += (val[step] == '(' ? 1 : -1);
            if (currentFloor < 0) return step + 1;
        }
        return 0;
    }

    PART_ONE() {
        //return Constexpr::ToString(FindFloor(Line));
        return Constexpr::ToString(FindFloor(lines[0]));
    }
    PART_TWO() {
        //return Constexpr::ToString(FindBasementStep(Line));
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