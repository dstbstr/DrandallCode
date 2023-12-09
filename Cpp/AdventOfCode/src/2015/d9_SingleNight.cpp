#pragma once

#include "2015/d9_SingleNight.h"

SOLUTION(2015, 9) {
    using DistanceMap = Constexpr::SmallMap<std::string, Constexpr::SmallMap<std::string, size_t>>;

    constexpr void ParseInput(const auto& lines, DistanceMap& outDistances, Constexpr::SmallSet<std::string>& outDestinations) {
        outDistances = {};
        outDestinations = {};

        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " = ");
            auto s2 = Constexpr::Split(s[0], " to ");
            size_t distance;
            Constexpr::ParseNumber(s[1], distance);
            auto from = std::string(s2[0]);
            auto to = std::string(s2[1]);

            outDistances[from][to] = distance;
            outDistances[to][from] = distance;
            outDestinations.insert(to);
            outDestinations.insert(from);
        }
    }

    constexpr size_t GetTotalDistance(const std::vector<std::string>& destinations, const DistanceMap& distances) {
        size_t result = 0;
        for (size_t i = 0; i < destinations.size() - 1; i++) {
            result += distances.at(destinations[i]).at(destinations[i + 1]);
        }
        return result;
    }

    constexpr size_t Solve(const auto& lines, bool min) {
        DistanceMap distances;
        Constexpr::SmallSet<std::string> destinations;
        ParseInput(lines, distances, destinations);

        auto path = std::vector<std::string>{ destinations.cbegin(), destinations.cend() };
        std::sort(path.begin(), path.end());

        size_t best = 0;
        if (min) {
            best = std::numeric_limits<size_t>::max();
        }

        do {
            if (min) {
                best = std::min(best, GetTotalDistance(path, distances));
            }
            else {
                best = std::max(best, GetTotalDistance(path, distances));
            }
        } while (std::next_permutation(path.begin(), path.end()));

        return best;
    }
    PART_ONE() {
        return Constexpr::ToString(Solve(lines, true));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, false));
    }

    TESTS() {
        std::vector<std::string> lines = {
            "London to Dublin = 464",
            "London to Belfast = 518",
            "Dublin to Belfast = 141"
        };

        if (Solve(lines, true) != 605) return false;

        return true;
    }
}
