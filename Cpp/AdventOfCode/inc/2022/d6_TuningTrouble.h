#pragma once

#include "Common.h"

constexpr u32 FindUnique(const std::string& input, size_t runLength) {
    std::vector<char> allChars;
    for (size_t i = 0; i < input.size() - runLength; i++) {
        allChars.clear();
        for (auto j = 0; j < runLength; j++) {
            if (std::find(allChars.begin(), allChars.end(), input[i + j]) == allChars.end()) {
                allChars.push_back(input[i + j]);
            }
            else {
                break;
            }
        }
        if (allChars.size() == runLength) {
            return static_cast<u32>(i + runLength);
        }
    }
    
    return 0;
}

constexpr u32 FindSignalStart(const std::string& input) {
    return FindUnique(input, 4);
}

static_assert(FindSignalStart("bvwbjplbgvbhsrlpgdmjqwftvncz") == 5);
static_assert(FindSignalStart("nppdvjthqldpwncqszvftbrmjlhg") == 6);
static_assert(FindSignalStart("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") == 10);
static_assert(FindSignalStart("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") == 11);

constexpr u32 FindMessageStart(const std::string& input) {
    return FindUnique(input, 14);
}

static_assert(FindMessageStart("mjqjpqmgbljsphdztnvjfqwrcgsmlb") == 19);

std::string Run(const std::vector<std::string>& lines) {
    return ToString(FindMessageStart(lines[0]));
}

bool RunTests() {
    if (FindMessageStart("mjqjpqmgbljsphdztnvjfqwrcgsmlb") != 19) return false;
    if(FindSignalStart("bvwbjplbgvbhsrlpgdmjqwftvncz") != 5) return false;
    if(FindSignalStart("nppdvjthqldpwncqszvftbrmjlhg") != 6) return false;
    if(FindSignalStart("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg") != 10) return false;
    if (FindSignalStart("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw") != 11) return false;
    return true;
}