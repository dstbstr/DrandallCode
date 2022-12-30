#pragma once

#include "Common.h"

constexpr std::vector<std::vector<u32>> ParseLines(const std::vector<std::string>& lines) {
    std::vector<std::vector<u32>> result;
    for (const auto& line : lines) {
        std::vector<u32> connections;
        auto values = Constexpr::Split(Constexpr::Split(line, " <-> ")[1], ", ");
        for (auto value : values) {
            u32 num;
            Constexpr::ParseNumber(value, num);
            connections.push_back(num);
        }
        result.push_back(connections);
    }

    return result;
}

static_assert(ParseLines(std::vector<std::string>{ "0 <-> 2" }).size() == 1);
static_assert(ParseLines(std::vector<std::string>{ "0 <-> 2"})[0].size() == 1);
static_assert(ParseLines(std::vector<std::string>{ "0 <-> 2"})[0][0] == 2);
static_assert(ParseLines(std::vector<std::string>{ "2 <-> 0, 3, 4"})[0].size() == 3);
static_assert(ParseLines(std::vector<std::string>{ "2 <-> 0, 3, 4"})[0] == std::vector<u32>{0, 3, 4});

constexpr std::vector<u32> FindGroup(u32 root, const std::vector<std::vector<u32>>& connections) {
    std::vector<u32> result;
    std::vector<u32> toVisit = connections[root];
    while (!toVisit.empty()) {
        auto next = toVisit.back();
        toVisit.pop_back();

        if (std::find(result.begin(), result.end(), next) != result.end()) continue;
        result.push_back(next);

        std::copy(connections[next].begin(), connections[next].end(), std::back_inserter(toVisit));
    }

    return result;
}

constexpr auto PartOne(const std::vector<std::string>& lines) {
    auto connections = ParseLines(lines);
    return FindGroup(0, connections).size();
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto connections = ParseLines(lines);
    std::vector<u32> remainingGroups;
    for (auto i = 0; i < connections.size(); i++) {
        remainingGroups.push_back(i);
    }

    u32 groupCount = 0;
    while (!remainingGroups.empty()) {
        groupCount++;
        auto group = FindGroup(remainingGroups.back(), connections);
        std::erase_if(remainingGroups, [&group](u32 val) {
            return std::find(group.cbegin(), group.cend(), val) != group.cend();
            });
    }

    return groupCount;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    return true;
}