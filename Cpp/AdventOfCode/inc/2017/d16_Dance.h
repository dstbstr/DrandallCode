#pragma once

#include "Common.h"

constexpr void Spin(std::string& dancers, std::string_view amountStr) {
    u32 amount;
    Constexpr::ParseNumber(amountStr, amount);
    std::rotate(dancers.rbegin(), dancers.rbegin() + amount, dancers.rend());
}

constexpr void Exchange(std::string& dancers, std::string_view details) {
    auto split = Constexpr::Split(details, "/");
    size_t lhs;
    size_t rhs;
    Constexpr::ParseNumber(split[0], lhs);
    Constexpr::ParseNumber(split[1], rhs);

    std::swap(dancers[lhs], dancers[rhs]);
}

constexpr void Partner(std::string& dancers, std::string_view details) {
    auto split = Constexpr::Split(details, "/");
    size_t lhs = dancers.find(split[0]);
    size_t rhs = dancers.find(split[1]);

    std::swap(dancers[lhs], dancers[rhs]);
}

constexpr void ApplyMove(std::string& dancers, std::string_view move) {
    auto type = move[0];
    auto details = move.substr(1);
    switch (type) {
    case 's': Spin(dancers, details); break;
    case 'x': Exchange(dancers, details); break;
    case 'p': Partner(dancers, details); break;
    default: break;
    }
}

//ogenlicadmpbjkhf is wrong
auto PartOne(const std::string& startingDancers, const std::string& line) {
    std::string dancers = startingDancers;
    auto split = Constexpr::Split(line, ",");
    for (auto move : split) {
        ApplyMove(dancers, move);
    }

    return dancers;
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    std::string dancers = "abcdefghijklmnop";
    return ToString(PartOne(dancers, lines[0]));
}

bool RunTests() {
    std::string dancers = "abcde";
    std::string line = "s1,x3/4,pe/b";
    if (PartOne(dancers, line) != "baedc") return false;

    dancers = "abcde";

    Spin(dancers, "3");
    if( dancers != "cdeab") return false;
    return true;
}