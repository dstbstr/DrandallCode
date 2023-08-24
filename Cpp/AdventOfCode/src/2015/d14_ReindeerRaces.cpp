#pragma once

#include "2015/d14_ReindeerRaces.h"

SOLUTION(2015, 14) {
    //Rudolph can fly 22 km/s for 8 seconds, but then must rest for 165 seconds.
    struct Reindeer
    {
        u32 Speed{ 0 };
        u32 RunTime{ 0 };
        u32 RestTime{ 0 };
    };

    constexpr Reindeer ParseRacer(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        Reindeer result;
        Constexpr::ParseNumber(s[3], result.Speed);
        Constexpr::ParseNumber(s[6], result.RunTime);
        Constexpr::ParseNumber(s[13], result.RestTime);

        return result;
    }

    constexpr u32 FindDistance(Reindeer who, u32 seconds) {
        u32 cycleTime = who.RunTime + who.RestTime;
        u32 cycles = seconds / cycleTime;
        u32 remainder = seconds % cycleTime;

        u32 distance = who.Speed * cycles * who.RunTime;

        if (remainder <= who.RunTime) {
            distance += who.Speed * remainder;
        }
        else {
            distance += who.Speed * who.RunTime;
        }

        return distance;
    }


    constexpr std::vector<u32> GetPoints(const std::vector<Reindeer>& racers, u32 seconds) {
        auto points = std::vector<u32>(racers.size(), 0);
        auto distances = std::vector<u32>(racers.size(), 0);
        for (u32 second = 1; second <= seconds; second++) {
            u32 best = 0;
            for (size_t index = 0; index < racers.size(); index++) {
                distances[index] = FindDistance(racers[index], second);
                best = std::max(best, distances[index]);
            }
            for (size_t index = 0; index < racers.size(); index++) {
                points[index] += distances[index] == best;
            }
        }

        return points;
    }

    PART_ONE() {
        //One liner, just for fun. :)
        return Constexpr::ToString(RunAllReturnMax(ParseLines(Lines, ParseRacer), FindDistance, 2503));
        /*
        auto racers = ParseLines<Reindeer>(lines, ParseRacer);
        u32 best = 0;
        for (const auto& racer : racers) {
            best = std::max(best, FindDistance(racer, 2503));
        }

        return Constexpr::ToString(best);
        */
    }
    PART_TWO() {
        auto racers = ParseLines(Lines, ParseRacer);
        auto points = GetPoints(racers, 2503);
        
        return Constexpr::ToString(*std::max_element(points.cbegin(), points.cend()));
    }

    TESTS() {
        static_assert(FindDistance({ 14,10,127 }, 10) == 140);
        static_assert(FindDistance({ 14,10,127 }, 11) == 140);
        static_assert(FindDistance({ 14,10,127 }, 1000) == 1120);

        static_assert(FindDistance({ 16,11,162 }, 10) == 160);
        static_assert(FindDistance({ 16,11,162 }, 11) == 176);
        static_assert(FindDistance({ 16,11,162 }, 1000) == 1056);

        return true;
    }
}