#pragma once

#include "Common.h"

using Memory = std::unordered_map<size_t, size_t>;

//mask = 110000011XX0000X101000X10X01XX001011
constexpr void UpdateMasks(std::string_view str, size_t& zeroMask, size_t& oneMask) {
    size_t value = 1;
    zeroMask = 0;
    oneMask = 0;
    for (auto it = str.rbegin(); it != str.rend(); it++) {
        if (*it == '1') oneMask += value;
        else if (*it == '0') zeroMask += value;
        value *= 2;
    }
}

/*
11000011XX0000X101000X10X01XX001011
00111100001111001011100101000110100 = 8084900404 (zeros)
11000011000000010100001000100001011 = 26173116683 (ones)
*/
constexpr bool TestUpdateMasks() {
    std::string mask = "11000011XX0000X101000X10X01XX001011";
    size_t zeroMask = 0;
    size_t oneMask = 0;

    UpdateMasks(mask, zeroMask, oneMask);
    if (zeroMask != 8084900404) return false;
    if (oneMask != 26173116683) return false;
    return true;
}

static_assert(TestUpdateMasks());

void Write(size_t val, size_t address, Memory& memory, size_t zeroMask, size_t oneMask) {
    val &= ~zeroMask;
    val |= oneMask;
    memory[address] = val;
}

void UpdateFloaters(size_t address, size_t floating, Memory& memory, size_t toWrite) {
    if (floating == 0) return;

    std::vector<size_t> seenValues{ 0 };
    size_t power = 1;
    while (power <= floating) {
        if ((floating & power) == power) {
            std::vector<size_t> toAdd;
            for (auto v : seenValues) {
                toAdd.push_back(v + power);
            }
            std::copy(toAdd.begin(), toAdd.end(), std::back_inserter(seenValues));
        }
        power *= 2;
    }

    for (auto v : seenValues) {
        memory[address + v] = toWrite;
    }
}

void Write2(size_t val, size_t address, Memory& memory, size_t zeroMask, size_t oneMask) {
    address |= oneMask;
    size_t floating = ~(zeroMask | oneMask);
    size_t floatingMask = ((~0ull) >> (64 - 36));
    floating &= floatingMask; //strip out 1s above 36 bits
    address &= (~floating); //erase floating bits from address

    UpdateFloaters(address, floating, memory, val);
}


size_t ProcessLines(const std::vector<std::string>& lines, auto writeFunc) {
    size_t zeroMask = 0;
    size_t oneMask = 0;
    Memory memory{};
    for (const auto& line : lines) {
        auto split = Constexpr::Split(line, " = ");
        if (split[0] == "mask") {
            UpdateMasks(split[1], zeroMask, oneMask);
        }
        else {
            size_t val, address;
            Constexpr::ParseNumber(split[1], val);
            Constexpr::ParseNumber(split[0].substr(3, split[0].size() - 4), address);
            writeFunc(val, address, memory, zeroMask, oneMask);
        }
    }

    size_t result = 0;
    for (const auto& [address, value] : memory) {
        result += value;
    }
    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(ProcessLines(lines, Write));
    return ToString(ProcessLines(lines, Write2));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
        "mem[8] = 11",
        "mem[7] = 101",
        "mem[8] = 0"
    };

    if (PartOne(lines) != 165) return false;

    lines = {
        "mask = 000000000000000000000000000000X1001X",
        "mem[42] = 100",
        "mask = 00000000000000000000000000000000X0XX",
        "mem[26] = 1"
    };
    if (PartTwo(lines) != 208) return false;

    lines = {
        "mask = 000000000000000000000000000000000XXX",
        "mem[8] = 4",
        "mask = XX0000000000000000000000000000000000",
        "mem[0] = 5"
    };
    if (PartTwo(lines) != 52) return false;

    return true;
}