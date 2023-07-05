#include "2022/d3_Rucksack.h"

SOLUTION(2022, 3) {
    constexpr u32 GetLetterValue(char letter) {
        if (letter <= 'Z') {
            return letter - 'A' + 27;
        }
        else {
            return letter - 'a' + 1;
        }
    }

    static_assert(GetLetterValue('a') == 1);
    static_assert(GetLetterValue('A') == 27);
    static_assert(GetLetterValue('z') == 26);
    static_assert(GetLetterValue('Z') == 52);

    constexpr char FindDuplicate(const std::string & line) {
        for (auto i = 0; i < line.size() / 2; i++) {
            for (auto j = line.size() / 2; j < line.size(); j++) {
                if (line[i] == line[j]) {
                    return line[i];
                }
            }
        }
        return '_';
    }

    static_assert(FindDuplicate("vJrwpWtwJgWrhcsFMMfFFhFp") == 'p');
    static_assert(FindDuplicate("jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL") == 'L');
    static_assert(FindDuplicate("PmmdzqPrVvPwwTWBwg") == 'P');
    static_assert(FindDuplicate("wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn") == 'v');

    u32 RunFirst(const std::vector<std::string>&lines) {
        u32 result = 0;
        for (auto& line : lines) {
            result += GetLetterValue(FindDuplicate(line));
        }
        return result;
    }

    constexpr char FindBadge(const std::string & first, const std::string & second, const std::string & third) {
        for (auto c = 'a'; c <= 'z'; c++) {
            if (std::find(first.begin(), first.end(), c) != first.end()) {
                if (std::find(second.begin(), second.end(), c) != second.end()) {
                    if (std::find(third.begin(), third.end(), c) != third.end()) {
                        return c;
                    }
                }
            }
        }

        for (auto c = 'A'; c <= 'Z'; c++) {
            if (std::find(first.begin(), first.end(), c) != first.end()) {
                if (std::find(second.begin(), second.end(), c) != second.end()) {
                    if (std::find(third.begin(), third.end(), c) != third.end()) {
                        return c;
                    }
                }
            }
        }

        return '_';
    }

    static_assert(FindBadge(
        "vJrwpWtwJgWrhcsFMMfFFhFp",
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
        "PmmdzqPrVvPwwTWBwg") == 'r');

    static_assert(FindBadge(
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
        "ttgJtRGJQctTZtZT",
        "CrZsJsPPZsGzwwsLwLmpwMDw") == 'Z');

    u32 Run(const std::vector<std::string>&lines) {
        u32 result = 0;
        for (auto i = 0; i < lines.size(); i += 3) {
            result += GetLetterValue(FindBadge(lines[i], lines[i + 1], lines[i + 2]));
        }

        return result;
    }

    bool RunTests() {

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