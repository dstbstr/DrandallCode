#pragma once

#include "Common.h"

#include <iostream>

//Sensor at x=220580, y=684270: closest beacon is at x=436611, y=263737

void ParseLine(const std::string& line, Coord& sensor, Coord& beacon) {
    static auto re = std::regex(R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))");
    std::smatch match;
    if (std::regex_search(line, match, re)) {
        ParseNumber(match[1].str(), sensor.X);
        ParseNumber(match[2].str(), sensor.Y);
        ParseNumber(match[3].str(), beacon.X);
        ParseNumber(match[4].str(), beacon.Y);
    }
}

std::vector<std::pair<Coord, Coord>> GetPairs(const std::vector<std::string>& lines) {
    std::vector<std::pair<Coord, Coord>> pairs;

    for (const auto& line : lines) {
        Coord sensor, beacon;
        ParseLine(line, sensor, beacon);
        pairs.push_back(std::make_pair(sensor, beacon));
    }

    return pairs;
}

constexpr u64 SignalFrequency(const Coord& coord) {
    return static_cast<u64>(coord.X) * u64(4'000'000) + coord.Y;
}

void MarkNonBeacons(const std::vector<std::pair<Coord, Coord>> pairs, std::unordered_set<s32>& seen, s32 row, s32 min, s32 max) {
    for (const auto& [source, beacon] : pairs) {
        auto mDist = MDistance(source, beacon);
        auto yDist = Constexpr::Abs(source.Y - row);
        auto xDist = mDist - yDist;

        for (auto x = std::max(min, source.X - xDist); x <= std::min(max, source.X + xDist); x++) {
            seen.insert(x);
        }
        if (xDist == 0) {
            seen.insert(source.X);
        }
    }

    for (const auto& [source, beacon] : pairs) {
        if (beacon.Y == row) {
            seen.erase(beacon.X);
        }
    }
}

u32 CountNonBeacons(const std::vector<std::string>& lines, s32 targetRow) {
    auto pairs = GetPairs(lines);
    std::unordered_set<s32> seen;

    MarkNonBeacons(pairs, seen, targetRow, -5000000, 5000000);

    return static_cast<u32>(seen.size());
}

void PrintSeen(const std::vector<std::pair<Coord, Coord >>& pairs, const std::unordered_set<s32>& seen, s32 row, u32 max) {
    for (s32 i = 0; i <= static_cast<s32>(max); i++) {
        bool printed = false;
        for (const auto& [source, beacon] : pairs) {
            if (source.X == i && source.Y == row) {
                std::cout << "S";
                printed = true;
                break;
            }
            else if (beacon.X == i && beacon.Y == row) {
                std::cout << "B";
                printed = true;
                break;
            }
        }
        if (!printed) {
            if (seen.find(i) == seen.end()) {
                std::cout << ".";
            }
            else {
                std::cout << "#";
            }
        }
    }
    std::cout << '\n';
}

s32 HasBeacon(const std::vector<std::pair<Coord, Coord>>& pairs, s32 row, s32 max) {
    static std::unordered_set<s32> seen = [max]() {
        std::unordered_set<s32> result;
        result.reserve(max);
        return result;
    }();

    seen.clear();
    MarkNonBeacons(pairs, seen, row, 0, max);

    //don't consider existing beacons
    for (const auto& [source, beacon] : pairs) {
        if (beacon.Y == row) {
            seen.insert(beacon.X);
        }
    }

    //PrintSeen(pairs, seen, row, max);
    if (seen.size() == max) {
        for (auto x = 0; x < max; x++) {
            if (seen.find(x) == seen.end()) {
                return x;
            }
        }
    }
    return 0;
}

Coord FindMissingBeacon(const std::vector<std::string>& lines, s32 max) {
    auto pairs = GetPairs(lines);
    for (s32 row = 0; row <= max; row++) {
        auto col = HasBeacon(pairs, row, max);
        if (col != 0) {
            return Coord{ col, row };
        }
    }

    return {};
}

struct Circle {
    Coord Center;
    s32 Radius;
};

std::vector<Circle> GetCircles(const std::vector<std::pair<Coord, Coord>> pairs) {
    std::vector<Circle> result;
    for (const auto& [source, beacon] : pairs) {
        result.push_back({ source, MDistance(source, beacon) });
    }

    return result;
}

constexpr bool Contains(const Circle& circle, const Coord& point) {
    return Constexpr::Abs(point.X - circle.Center.X) + Constexpr::Abs(point.Y - circle.Center.Y) <= circle.Radius;
}

static_assert(Contains({ {0, 0}, 5 }, { 1, 1 }));
static_assert(Contains({ {0, 11}, 3 }, { 0, 8 }));
static_assert(!Contains({ {0, 0}, 2 }, { 5, 5 }));
static_assert(!Contains({ {8, 7}, 9 }, { 14, 11 }));

constexpr void Translate(Coord& coord, const Circle& circle) {
    coord.X += circle.Center.X;
    coord.Y += circle.Center.Y;
}

constexpr std::vector<Coord> GetAdjacentPoints(const Circle& circle, s32 min, s32 max) {
    std::vector<Coord> result;
    auto rad = circle.Radius;
    std::vector<Coord> dir;
    dir.resize(4);
    for (auto i = 0; i < rad; i++) {
        dir[0] = { 0 + i, 1 + rad - i};
        dir[1] = { 0 - i, -1 -rad + i};
        dir[2] = { -1 -rad + i, 0 - i};
        dir[3] = { 1 + rad - i, 0 + i};
        std::transform(dir.begin(), dir.end(), dir.begin(), [&circle](Coord coord) {
            return Coord{ coord.X + circle.Center.X, coord.Y + circle.Center.Y };
            });

        std::copy_if(dir.begin(), dir.end(), std::back_inserter(result), [max, min](Coord coord) {
            return coord.X >= min && coord.X <= max && coord.Y >= min && coord.Y <= max;
            });
    }

    return result;
}

Coord FindMissing(const std::vector<std::string>& lines, s32 min, s32 max) {
    auto circles = GetCircles(GetPairs(lines));
    std::sort(circles.begin(), circles.end(), [](const Circle& lhs, const Circle& rhs) {
        return lhs.Radius < rhs.Radius;
        });
    std::unordered_set<Coord, CoordHash> candidates;
    for (size_t i = 0; i < circles.size(); i++) {
        std::cout << "Checking circle " << (i + 1) << " of " << circles.size();
        candidates.clear();
        auto adjacent = GetAdjacentPoints(circles[i], min, max);
        candidates.insert(adjacent.cbegin(), adjacent.cend());
        std::cout << " with " << candidates.size() << " Candidates\n";

        for (auto candidate : candidates) {
            bool contained = false;
            for (size_t j = 0; j < circles.size(); j++) {
                if (i == j) continue;
                if (Contains(circles[j], candidate)) {
                    contained = true;
                    break;
                }
            }
            if (!contained) {
                std::cout << "Candidate Found! {" << candidate.X << "," << candidate.Y << "}\n";
                return candidate;
            }
        }
    }

    return {};
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(CountNonBeacons(lines, 2'000'000));
    s32 min = 2'000'000; //trial and error
    s32 max = 4'000'000;

    auto missingCoord = FindMissing(lines, min, max);
    return ToString(SignalFrequency(missingCoord));
}

bool RunTests() {
    std::vector < std::string> lines = {
        "Sensor at x=2, y=18: closest beacon is at x=-2, y=15",
        "Sensor at x=9, y=16: closest beacon is at x=10, y=16",
        "Sensor at x=13, y=2: closest beacon is at x=15, y=3",
        "Sensor at x=12, y=14: closest beacon is at x=10, y=16",
        "Sensor at x=10, y=20: closest beacon is at x=10, y=16",
        "Sensor at x=14, y=17: closest beacon is at x=10, y=16",
        "Sensor at x=8, y=7: closest beacon is at x=2, y=10",
        "Sensor at x=2, y=0: closest beacon is at x=2, y=10",
        "Sensor at x=0, y=11: closest beacon is at x=2, y=10",
        "Sensor at x=20, y=14: closest beacon is at x=25, y=17",
        "Sensor at x=17, y=20: closest beacon is at x=21, y=22",
        "Sensor at x=16, y=7: closest beacon is at x=15, y=3",
        "Sensor at x=14, y=3: closest beacon is at x=15, y=3",
        "Sensor at x=20, y=1: closest beacon is at x=15, y=3"
    };

    //if (CountNonBeacons(lines, 10) != 26) return false;
    //auto missingCoord = FindMissingBeacon(lines, 20);
    auto missingCoord = FindMissing(lines, 0, 20);
    if (missingCoord.X != 14) return false;
    if (missingCoord.Y != 11) return false;
    if (SignalFrequency(missingCoord) != 56000011) return false;


    return true;
}