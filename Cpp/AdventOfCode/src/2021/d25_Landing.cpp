#include "2021/d25_Landing.h"

SOLUTION(2021, 25) {
    constexpr std::vector<std::string> Next(const std::vector<std::string>&current) {
        std::vector<std::string> result;
        for (size_t row = 0; row < current.size(); row++) {
            result.push_back(std::string(current[row].size(), '.'));
        }
        for (size_t row = 0; row < current.size(); row++) {
            for (size_t col = 0; col < current[row].size(); col++) {
                auto c = current[row][col];
                if (c != '>') continue;
                auto nextCol = (col + 1) % current[row].size();
                if (current[row][nextCol] == '.') {
                    result[row][nextCol] = '>';
                }
                else {
                    result[row][col] = '>';
                }
            }
        }
        for (size_t row = 0; row < current.size(); row++) {
            for (size_t col = 0; col < current[row].size(); col++) {
                auto c = current[row][col];
                if (c != 'v') continue;
                auto nextRow = (row + 1) % current.size();
                if (current[nextRow][col] == 'v' || result[nextRow][col] == '>') {
                    result[row][col] = 'v';
                }
                else {
                    result[nextRow][col] = 'v';
                }
            }
        }
        return result;
    }

    PART_ONE() {
        auto current = CopyToVector(lines);
        size_t steps = 0;
        while (true) {
            steps++;
            auto next = Next(current);
            if (next == current) break;
            current = next;
        }

        return Constexpr::ToString(steps);
    }

    PART_TWO() {
        (void)lines;
        return "Merry Christmas!";
    }

    TESTS() {
        static_assert(Next({ "...>>>>>..." })[0] == "...>>>>.>..");
        static_assert(Next({ "...>>>>.>.." })[0] == "...>>>.>.>.");
        static_assert(Next({
            "...>...",
            ".......",
            "......>",
            "v.....>",
            "......>",
            ".......",
            "..vvv..",
            }) == std::vector<std::string> {
            "..vv>..",
                ".......",
                ">......",
                "v.....>",
                ">......",
                ".......",
                "....v..",
        });
 
        /*
        static_assert(Part1({
            "v...>>.vv>",
            ".vv>>.vv..",
            ">>.>v>...v",
            ">>v>>.>.v.",
            "v>v.vv.v..",
            ">.>>..v...",
            ".vv..>.>v.",
            "v.v..>>v.v",
            "....v..v.>",
            }) == 58);
        */

        return true;
    }
}