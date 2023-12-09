#include "2016/d16_DragonChecksum.h"

SOLUTION(2016, 16) {
    constexpr std::string Next(const std::string & value) {
        std::string next = value;
        for (size_t i = 0; i < next.size(); i++) {
            next[i] = next[i] == '0' ? '1' : '0';
        }

        std::reverse(next.begin(), next.end());
        return value + '0' + next;
    }

    constexpr std::string Checksum(const std::string & value) {
        std::string running = value;
        while (running.size() % 2 == 0) {
            std::string next = "";
            for (size_t i = 0; i < running.size(); i += 2) {
                next += running[i] == running[i + 1] ? '1' : '0';
            }
            running = next;
        }

        return running;
    }

    constexpr std::string FillDisk(size_t size, std::string_view initial) {
        std::string running = std::string(initial);
        while (running.size() <= size) {
            running = Next(running);
        }

        return Checksum(running.substr(0, size));
    }

    PART_ONE() {
        return FillDisk(272, lines[0]);

    }
    PART_TWO() {
        return FillDisk(35651584, lines[0]);

    }

    TESTS() {
        static_assert(Next("1") == "100");
        static_assert(Next("0") == "001");
        static_assert(Next("11111") == "11111000000");
        static_assert(Next("111100001010") == "1111000010100101011110000");

        static_assert(Checksum("110010110100") == "100");

        static_assert(FillDisk(272, "01000100010010111") == "10010010110011010");

        if (FillDisk(20, "10000") != "01100") return false;

        return true;
    }
}

