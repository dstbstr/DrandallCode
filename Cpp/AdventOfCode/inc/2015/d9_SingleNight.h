#pragma once

#include "Platform/Types.h"
#include "Utilities/StringUtils.h"
#include "Utils.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class TravelingSalesman {
    std::unordered_map<std::string, std::unordered_map<std::string, u32>> Distances;
    std::unordered_set<std::string> AllDestinations;

public:
    void AddDestination(std::string line) {
        auto firstSplit = StrUtil::Split(line, "=");
        auto secondSplit = StrUtil::Split(firstSplit[0], "to");

        u32 distance;
        ParseNumber(StrUtil::Trim(std::string(firstSplit[1])), distance);

        auto lhs = StrUtil::Trim(std::string(secondSplit[0]));
        auto rhs = StrUtil::Trim(std::string(secondSplit[1]));

        Distances[lhs][rhs] = distance;
        Distances[rhs][lhs] = distance;

        AllDestinations.insert(lhs);
        AllDestinations.insert(rhs);
    }

    u32 CalculatePath(std::vector<std::string>& path) {
        u32 distance = 0;
        std::string current;
        std::string next;

        for(auto i = 0; i < path.size() - 1; i++) {
            current = path[i];
            next = path[i + 1];
            distance += Distances[current][next];
        }

        return distance;
    }

    u32 FindShortestPath() {
        auto path = std::vector<std::string>{AllDestinations.begin(), AllDestinations.end()};
        std::sort(path.begin(), path.end());
        u32 result = 999999;

        do {
            result = std::min(result, CalculatePath(path));
        } while(std::next_permutation(path.begin(), path.end()));

        return result;
    }

    u32 FindLongestPath() {
        auto path = std::vector<std::string>{AllDestinations.begin(), AllDestinations.end()};
        std::sort(path.begin(), path.end());
        u32 result = 0;

        do {
            result = std::max(result, CalculatePath(path));
        } while(std::next_permutation(path.begin(), path.end()));

        return result;

    }

    bool RunTests() {
        AddDestination("London to Dublin = 464");
        AddDestination("London to Belfast = 518");
        AddDestination("Dublin to Belfast = 141");

        return FindShortestPath() == 605;
    }
};