#include "2016/d21_Scrambler.h"

SOLUTION(2016, 21) {
    constexpr std::array<size_t, 8> ShiftTable = { 1, 1, 6, 2, 7, 3, 0, 4 };

    constexpr void RunCommand(std::string_view line, bool scramble, std::string & running) {
        auto s = Constexpr::Split(line, " ");
        u32 n1, n2;

        if (s[0] == "swap") {
            if (s[1] == "position") {
                Constexpr::ParseNumber(s[2], n1);
                Constexpr::ParseNumber(s[5], n2);
                std::swap(running[n1], running[n2]);
            }
            else {
                auto c1 = s[2][0];
                auto c2 = s[5][0];
                std::swap(running[running.find(c1)], running[running.find(c2)]);
            }
        }
        else if (s[0] == "reverse") {
            Constexpr::ParseNumber(s[2], n1);
            Constexpr::ParseNumber(s[4], n2);
            std::reverse(running.begin() + n1, running.begin() + n2 + 1);
        }
        else if (s[0] == "rotate") {
            if (s[1] == "based") {
                auto index = running.find(s.back()[0]);
                if (scramble) {
                    auto rot = index + 1;
                    if (index >= 4) rot++;
                    rot %= running.size();
                    std::rotate(running.rbegin(), running.rbegin() + rot, running.rend());
                }
                else {
                    std::rotate(running.begin(), running.begin() + ShiftTable[index], running.end());
                }
            }
            else {
                Constexpr::ParseNumber(s[2], n1);
                
                if (scramble == (s[1] == "left")) {
                    std::rotate(running.begin(), running.begin() + n1, running.end());
                }
                else {
                    std::rotate(running.rbegin(), running.rbegin() + n1, running.rend());
                }
            }
        }
        else if (s[0] == "move") {
            Constexpr::ParseNumber(s[2], n1);
            Constexpr::ParseNumber(s[5], n2);
            auto left = scramble ? n1 : n2;
            auto right = scramble ? n2 : n1;

            if (left < right) {
                running.insert(running.begin() + right + 1, running[left]);
                running.erase(running.begin() + left);
            }
            else {
                running.insert(running.begin() + right, running[left]);
                running.erase(running.begin() + left + 1);
            }

        }
    }

    constexpr std::string Scramble(const auto& lines, std::string_view initial) {
        std::string running = std::string(initial);
        for (const auto& line : lines) {
            RunCommand(line, true, running);
        }
        return running;
    }
    constexpr std::string UnScramble(const auto& lines, std::string_view initial) {
        std::string running = std::string(initial);
        for (s32 i = static_cast<s32>(lines.size()) - 1; i >= 0; i--) {
            RunCommand(lines[i], false, running);
        }
        return running;
    }

    PART_ONE() {
        return Scramble(Lines, "abcdefgh");
    }
    PART_TWO() {
        return UnScramble(Lines, "fbgdceah");
    }

    TESTS() {
        std::vector<std::string> lines = {
            "swap position 4 with position 0",
            "swap letter d with letter b",
            "reverse positions 0 through 4",
            "rotate left 1 step",
            "move position 1 to position 4",
            "move position 3 to position 0",
            "rotate based on position of letter b",
            "rotate based on position of letter d"
        };

        if (Scramble(lines, "abcde") != "decab") return false;
        if (UnScramble(lines, "decab") != "abcde") return false;
        
        return true;
    }
}