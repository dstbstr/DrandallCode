#pragma once

#include "Platform/Types.h"
#include "Utils.h"

#include <regex>
#include <string>
#include <array>

struct Reindeer
{
    u32 Speed;
    u32 RunTime;
    u32 RestTime;
};

constexpr u32 FindDistance(Reindeer who, u32 seconds) {
    u32 cycleTime = who.RunTime + who.RestTime;
    u32 cycles = seconds / cycleTime;
    u32 remainder = seconds % cycleTime;

    u32 distance = who.Speed * cycles * who.RunTime;

    if(remainder <= who.RunTime) {
        distance += who.Speed * remainder;
    } else {
        distance += who.Speed * who.RunTime;
    }

    return distance;
}

static_assert(FindDistance({14,10,127}, 10) == 140);
static_assert(FindDistance({14,10,127}, 11) == 140);
static_assert(FindDistance({14,10,127}, 1000) == 1120);

static_assert(FindDistance({16,11,162}, 10) == 160);
static_assert(FindDistance({16,11,162}, 11) == 176);
static_assert(FindDistance({16,11,162}, 1000) == 1056);

template<size_t N>
constexpr std::array<u32, N> GetPoints(std::array<Reindeer, N> racers, u32 seconds) {
    auto points = std::array<u32, N>{};
    for(u32 second = 1; second <= seconds; second++) {
        auto distances = std::array<u32, N>{};
        u32 maxDistance = 0;
        for(size_t index = 0; index < racers.size(); index++) {
            distances[index] = FindDistance(racers[index], second);
            maxDistance = std::max(maxDistance, distances[index]);
        }

        for(size_t index = 0; index < distances.size(); index++) {
            if(distances[index] == maxDistance) {
                points[index]++;
            }
        }
    }

    return points;
}

Reindeer CreateRacer(std::string line) {
    static auto re = std::regex(R"([^\d]+(\d+) [^\d]+(\d+)[^\d]+(\d+))");
    auto match = std::smatch{};
    std::regex_search(line, match, re);

    u32 speed;
    u32 runTime;
    u32 restTime;

    ParseNumber(match[1].str(), speed);
    ParseNumber(match[2].str(), runTime);
    ParseNumber(match[3].str(), restTime);

    return {speed, runTime, restTime};
}


bool RunTests() {
    return true;
}