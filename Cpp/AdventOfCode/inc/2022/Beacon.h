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
    return coord.X * 4'000'000 + coord.Y;
}

u32 CountNonBeacons(const std::vector<std::string>& lines, s32 targetRow) {
    auto pairs = GetPairs(lines);
    std::unordered_set<s32> seen;

    for (const auto& [source, beacon] : pairs) {
        auto mDist = MDistance(source, beacon);
        auto yDist = Constexpr::Abs(source.Y - targetRow);
        auto xDist = mDist - yDist;

        for (auto x = source.X - xDist; x <= source.X + xDist; x++) {
            seen.insert(x);
        }
        if (xDist == 0) {
            seen.insert(source.X);
        }
    }

    for (const auto& [source, beacon] : pairs) {
        if (beacon.Y == targetRow) {
            seen.erase(beacon.X);
        }
    }

    return static_cast<u32>(seen.size());
}

s32 HasBeacon(const std::vector<std::pair<Coord, Coord>>& pairs, u32 row, u32 max) {
    static std::unordered_set<s32> seen = [max]() {
        std::unordered_set<s32> result;
        //result.reserve(4'000'001);
        result.reserve(max);
        return result;
    }();

    seen.clear();
    for (const auto& [source, beacon] : pairs) {
        auto mDist = MDistance(source, beacon);
        auto yDist = Constexpr::Abs(source.Y - row);
        if (yDist >= mDist) continue;
        auto xDist = mDist - yDist;

        for (s32 x = std::max(0u, source.X - xDist); x <= std::min(max, source.X + xDist); x++) {
            seen.insert(x);
        }
        if (xDist == 0) {
            seen.insert(source.X);
        }
    }

    for (const auto& [source, beacon] : pairs) {
        if (beacon.Y == row) {
            seen.insert(beacon.X);
        }
    }

    if (seen.size() < max) {
        for (auto x = 0; x < max; x++) {
            if (seen.find(x) == seen.end()) {
                return x;
            }
        }
    }
    return 0;
}

Coord FindMissingBeacon(const std::vector<std::string>& lines, u32 max) {
    auto pairs = GetPairs(lines);
    for (auto row = 0; row <= max; row++) {
        auto col = HasBeacon(pairs, row, max);
        if (col != 0) {
            return Coord{ col, row };
        }
    }

    return {};
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(CountNonBeacons(lines, 2'000'000));
    u32 max = 4'000'000;
    auto missingCoord = FindMissingBeacon(lines, max);
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
        "Sensor at x=20, y=1: closest beacon is at x=15, y= 3"
    };

    //if (CountNonBeacons(lines, 10) != 26) return false;
    auto missingCoord = FindMissingBeacon(lines, 20);
    if (missingCoord.X != 14) return false;
    if (missingCoord.Y != 11) return false;
    if (SignalFrequency(missingCoord) != 56000011) return false;

    return true;
}