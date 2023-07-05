#include "2018/d5_Reduction.h"

SOLUTION(2018, 5) {
    constexpr static auto Delta = 'a' - 'A';

    constexpr bool Reduce(std::string & input) {
        if (input.empty()) return false;

        bool shrunk = false;
        std::string result;
        for (auto i = 0; i < input.size() - 1; i++) {
            auto delta = (input[i] > input[i + 1] ? input[i] - input[i + 1] : input[i + 1] - input[i]);
            if (delta != Delta) {
                result.push_back(input[i]);
            }
            else {
                i++;
                shrunk = true;
            }
        }

        if (result.size() % 2 == 1) {
            result.push_back(input[input.size() - 1]);
        }

        input = result;
        return shrunk;
    }

    constexpr std::string FullReduce(const std::string & input) {
        std::string running = input;
        while (Reduce(running));

        return running;
    }

    static_assert(FullReduce("aA") == "");
    static_assert(FullReduce("aa") == "aa");
    static_assert(FullReduce("dabAcCaCBAcCcaDA") == "dabCBAcaDA");

    constexpr auto Part1(const std::string & line) {
        return FullReduce(line).size();
    }

    constexpr auto Part2(const std::string & line) {
        size_t best = 9999;

        for (auto c = 'a'; c <= 'z'; c++) {
            auto test = line;
            test.erase(std::remove(test.begin(), test.end(), c), test.end());
            test.erase(std::remove(test.begin(), test.end(), c - Delta), test.end());

            best = std::min(best, FullReduce(test).size());
        }

        return best;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines[0]));
        return Constexpr::ToString(Part2(lines[0]));
    }

    bool RunTests() {
        if (Part2("dabAcCaCBAcCcaDA") != 4) return false;
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