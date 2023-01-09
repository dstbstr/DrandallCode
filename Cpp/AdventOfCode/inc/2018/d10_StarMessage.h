#pragma once

#include "Common.h"
#include <iostream>

struct Star {
    Coord Pos;
    Coord Vel;
};

constexpr Star ParseLine(const std::string& line) {
    const char* numberSearch = "1234567890-";

    auto s1 = Constexpr::Split(line, "=");
    auto posStr = s1[1].substr(1, s1[1].find('>') - 1);
    auto velStr = s1[2].substr(1, s1[2].find('>') - 1);

    auto pStart = posStr.find_first_of(numberSearch);
    auto pComma = posStr.find(", ");
    auto px = posStr.substr(pStart, pComma - pStart);
    auto py = posStr.substr(posStr.find_first_of(numberSearch, pComma));

    auto vStart = velStr.find_first_of(numberSearch);
    auto vComma = velStr.find(", ");
    auto vx = velStr.substr(vStart, vComma - vStart);
    auto vy = velStr.substr(velStr.find_first_of(numberSearch, vComma));

    Star result;
    Constexpr::ParseNumber(px, result.Pos.X);
    Constexpr::ParseNumber(py, result.Pos.Y);
    Constexpr::ParseNumber(vx, result.Vel.X);
    Constexpr::ParseNumber(vy, result.Vel.Y);

    return result;
}

constexpr void TickStars(std::vector<Star>& stars, u32 ticks) {
    for (auto& star : stars) {
        star.Pos.X += star.Vel.X * ticks;
        star.Pos.Y += star.Vel.Y * ticks;
    }
}

constexpr void GetLimits(const std::vector<Star> stars, Coord& min, Coord& max) {
    min = { 9999, 9999 };
    max = { -9999, -9999 };
    for (const auto& star : stars) {
        min.X = std::min(min.X, star.Pos.X);
        min.Y = std::min(min.Y, star.Pos.Y);
        max.X = std::max(max.X, star.Pos.X);
        max.Y = std::max(max.Y, star.Pos.Y);
    }
}

constexpr bool HasStar(const std::vector<Star>& stars, Coord pos) {
    for (const auto& star : stars) {
        if (star.Pos == pos) {
            return true;
        }
    }

    return false;
}

bool PrintStarsIfFits(const std::vector<Star>& stars) {
    Coord min, max;
    GetLimits(stars, min, max);
    if (Constexpr::Abs(max.Y - min.Y) > 10) return false;
    if (Constexpr::Abs(max.X - min.X) > 100) return false;

    for (auto y = min.Y; y <= max.Y; y++) {
        for (auto x = min.X; x <= max.X; x++) {
            Coord pos = { x, y };
            if (HasStar(stars, pos)) {
                std::cout << '#';
            } 
            else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }

    std::cout << '\n';
    return true;
}

auto PartOne(const std::vector<std::string>& lines) {
    std::vector<Star> stars;
    for (const auto& line : lines) {
        stars.push_back(ParseLine(line));
    }

    u32 seconds = 0;
    if (stars[0].Vel.X > 0) {
        seconds = Constexpr::Abs(stars[0].Pos.Y / stars[0].Vel.Y) - 50;
        TickStars(stars, seconds);
    }

    bool printed = false;
    while (true) {
        if (PrintStarsIfFits(stars)) {
            printed = true;
        }
        else if (printed) {
            break;
        }

        TickStars(stars, 1);
        seconds++;
    }

    return seconds - 1;
}

auto PartTwo(const std::vector<std::string>& lines) {
    return lines.size();
}

std::string Run(const std::vector<std::string>& lines) {
    return ToString(PartOne(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "position=< 9,  1> velocity=< 0,  2>",
        "position=< 7,  0> velocity=<-1,  0>",
        "position=< 3, -2> velocity=<-1,  1>",
        "position=< 6, 10> velocity=<-2, -1>",
        "position=< 2, -4> velocity=< 2,  2>",
        "position=<-6, 10> velocity=< 2, -2>",
        "position=< 1,  8> velocity=< 1, -1>",
        "position=< 1,  7> velocity=< 1,  0>",
        "position=<-3, 11> velocity=< 1, -2>",
        "position=< 7,  6> velocity=<-1, -1>",
        "position=<-2,  3> velocity=< 1,  0>",
        "position=<-4,  3> velocity=< 2,  0>",
        "position=<10, -3> velocity=<-1,  1>",
        "position=< 5, 11> velocity=< 1, -2>",
        "position=< 4,  7> velocity=< 0, -1>",
        "position=< 8, -2> velocity=< 0,  1>",
        "position=<15,  0> velocity=<-2,  0>",
        "position=< 1,  6> velocity=< 1,  0>",
        "position=< 8,  9> velocity=< 0, -1>",
        "position=< 3,  3> velocity=<-1,  1>",
        "position=< 0,  5> velocity=< 0, -1>",
        "position=<-2,  2> velocity=< 2,  0>",
        "position=< 5, -2> velocity=< 1,  2>",
        "position=< 1,  4> velocity=< 2,  1>",
        "position=<-2,  7> velocity=< 2, -2>",
        "position=< 3,  6> velocity=<-1, -1>",
        "position=< 5,  0> velocity=< 1,  0>",
        "position=<-6,  0> velocity=< 2,  0>",
        "position=< 5,  9> velocity=< 1, -2>",
        "position=<14,  7> velocity=<-2,  0>",
        "position=<-3,  6> velocity=< 2, -1>"
    };

    PartOne(lines);

    return true;
}