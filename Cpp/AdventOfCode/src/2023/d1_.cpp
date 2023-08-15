#include "2023/d1_.h"

SOLUTION(2023, 1) {
    PART_ONE() {
        (void)lines;
        if (input.empty()) {
            return "Couldn't Find Input";
        }
        else {
            return std::string(inputLines[0]);
        }
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}