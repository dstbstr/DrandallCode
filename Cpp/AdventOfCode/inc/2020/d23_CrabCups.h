#pragma once

#include "Common.h"

constexpr std::vector<u32> ParseLine(const std::string& line) {
    std::vector<u32> cups;
    for (const auto& c : line) {
        cups.push_back(c - '0');
    }
    return cups;
}

static_assert(ParseLine("1234") == std::vector<u32>{1, 2, 3, 4});

constexpr void DoMove(std::vector<u32>& cups) {
    auto current = cups[0];
    auto pickedUp = std::vector<u32>{ cups.begin() + 1, cups.begin() + 4 };
    cups.erase(cups.begin() + 1, cups.begin() + 4);
    auto target = current - 1;
    while (std::find(cups.begin(), cups.end(), target) == cups.end()) {
        if (target == 0) {
            target = 9;
        }
        else {
            target--;
        }
    }

    auto targetIndex = std::find(cups.cbegin(), cups.cend(), target);
    cups.insert(targetIndex + 1, pickedUp.begin(), pickedUp.end());
    std::rotate(cups.begin(), cups.begin() + 1, cups.end());
}

auto PartOne(const std::string& line, size_t rounds) {
    auto cups = ParseLine(line);
    for (size_t round = 0; round < rounds; round++) {
        DoMove(cups);
    }

    auto oneIndex = std::find(cups.begin(), cups.end(), 1u);
    std::rotate(cups.begin(), oneIndex, cups.end());
    std::string result = "";
    for (auto i = 1; i < cups.size(); i++) {
        result.push_back(static_cast<char>(cups[i] + '0'));
    }

    return result;
}

std::vector<size_t> ParseCupsTwo(const std::string& line) {
    std::vector<size_t> result;
    result.reserve(1'000'000);
    for (const auto c : line) {
        result.push_back(c - '0');
    }
    for (size_t i = 10; i <= 1'000'000; i++) {
        result.push_back(i);
    }

    return result;
}

void DoMove2(std::vector<size_t>& cups) {
    auto current = cups[0];
    auto pickedUp = std::vector<size_t>{ cups.begin() + 1, cups.begin() + 4 };
    cups.erase(cups.begin() + 1, cups.begin() + 4);
    auto target = current - 1;
    while (std::find(cups.begin(), cups.end(), target) == cups.end()) {
        if (target == 0) {
            target = cups.size();
        }
        else {
            target--;
        }
    }

    auto targetIndex = std::find(cups.cbegin(), cups.cend(), target);
    cups.insert(targetIndex + 1, pickedUp.begin(), pickedUp.end());
    std::rotate(cups.begin(), cups.begin() + 1, cups.end());
}

auto PartTwo(const std::string& line) {
    //389125467
    //cups[0] = 3
    //cups[1] = 2
    //cups[2] = 5
    //cups[3] = 8
    //cups[4] = 6
    //cups[5] = 4
    //cups[6] = 7
    //cups[7] = 3
    //cups[8] = 9
    //cups[9] = 1
    constexpr size_t Max = 1'000'000;
    std::vector<size_t> cups;
    cups.resize(Max + 1);
    size_t first = line[0] - '0';
    size_t prev = 0;
    size_t next = 0;
    cups[0] = first;
    for (auto i = 1; i < line.size(); i++) {
        prev = line[i - 1] - '0';
        next = line[i] - '0';
        cups[prev] = next;
    }

    prev = next;
    next = static_cast<u32>(line.size()) + 1;
    while (prev <= Max) {
        cups[prev] = next;
        prev = next;
        next++;
    }
    cups.back() = cups[0];

    for (auto round = 0; round < 10'000'000; round++) {
        auto r1 = cups[cups[0]];
        auto r2 = cups[r1];
        auto r3 = cups[r2];
        auto dest = cups[0] - 1;
        while (dest == r1 || dest == r2 || dest == r3 || dest == 0) {
            if (dest == 0) dest = Max;
            else dest--;
        }

        cups[cups[0]] = cups[r3];
        auto after = cups[dest];
        cups[dest] = r1;
        cups[r3] = after;
        cups[0] = cups[cups[0]];
    }
    return cups[1] * cups[cups[1]];
}

std::string Run(const std::vector<std::string>&) {
    //return PartOne("952438716", 100);
    return ToString(PartTwo("952438716"));
}

bool RunTests() {
    std::string line = "389125467";
    if (PartOne(line, 10) != "92658374") return false;
    if (PartOne(line, 100) != "67384529") return false;
    if (PartTwo(line) != 149245887792) return false;
    return true;
}