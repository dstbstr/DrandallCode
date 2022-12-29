#pragma once

#include "Common.h"
#include "Utilities/ToHex.h"

constexpr void Twist(std::vector<u32>& numbers, u32 length, u32& skip, u32& currentPos) {
    std::reverse(numbers.begin(), numbers.begin() + length);
    currentPos += length + skip;
    currentPos %= numbers.size();
    std::rotate(numbers.begin(), numbers.begin() + ((length + skip) % numbers.size()), numbers.end());
    skip++;
}

constexpr std::vector<u32> GetLengths(const std::string& input) {
    std::vector<u32> lengths;
    for (auto c : input) {
        lengths.push_back(static_cast<u32>(c));
    }
    lengths.insert(lengths.end(), { 17, 31, 73, 47, 23 });

    return lengths;
}

constexpr std::vector<u32> GetInitialNumbers() {
    std::vector<u32> numbers;
    for (auto i = 0; i < 256; i++) {
        numbers.push_back(i);
    }

    return numbers;
}

constexpr u32 Condense(const std::vector<u32>& sixteen) {
    u32 val = 0;
    for (auto j = 0; j < 16; j++) {
        val ^= sixteen[j];
    }

    return val;
}

static_assert(Condense({ 65, 27, 9, 1, 4, 3, 40, 50, 91, 7, 6, 0, 2, 5, 68, 22 }) == 64);

constexpr std::vector<u32> CondenseHash(const std::vector<u32>& sparseHash) {
    std::vector<u32> result;

    for (auto i = 0; i < 16; i++) {
        auto start = i * 16;
        std::vector<u32> subHash;
        std::copy_n(sparseHash.begin() + start, 16, std::back_inserter(subHash));
        result.push_back(Condense(subHash));
    }

    return result;
}

constexpr std::string HashToHex(const std::vector<u32>& denseHash) {
    std::string result;
    
    for (auto val : denseHash) {
        result += ToHexLower(static_cast<char>(val));
    }

    return result;
}

static_assert(HashToHex({ 64, 7 , 255 }) == "4007ff");

auto PartOne(const std::vector<u32>& lengths) {
    auto numbers = GetInitialNumbers();

    u32 skip = 0;
    u32 currentPos = 0;

    for (auto length : lengths) {
        Twist(numbers, length, skip, currentPos);
    }

    std::rotate(numbers.rbegin(), numbers.rbegin() + currentPos, numbers.rend());
    return numbers[0] * numbers[1];
}

auto PartTwo(const std::string& line) {
    auto numbers = GetInitialNumbers();
    auto lengths = GetLengths(line);
    u32 skip = 0;
    u32 currentPos = 0;
    
    for (auto i = 0; i < 64; i++) {
        for (auto length : lengths) {
            Twist(numbers, length, skip, currentPos);
        }
    }

    std::rotate(numbers.rbegin(), numbers.rbegin() + currentPos, numbers.rend());
    return HashToHex(CondenseHash(numbers));
}

std::string Run(const std::vector<std::string>&) {
    std::vector<u32> input = { 212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164 };

    //return ToString(PartOne(input));
    return ToString(PartTwo("212,254,178,237,2,0,1,54,167,92,117,125,255,61,159,164"));
}

bool RunTests() {
    if (PartTwo("1,2,3") != "3efbe78a8d82f29979031a4aa0b16a9d") return false;
    return true;
}