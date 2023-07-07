#pragma once
#include <vector>
#include <functional>
#include <algorithm>

//Given a starting point, and a neighbor func which returns an iterable of T,
//Returns all points filled
template<typename T>
constexpr std::vector<T> FloodFill(T start, auto NeighborFunc) {
    Constexpr::Set<T> seen{ start };
    std::vector<T> q{ start };
    std::vector<T> result;

    while (!q.empty()) {
        auto current = q.back();
        q.pop_back();
        result.push_back(current);

        auto neighbors = NeighborFunc(current);
        for (const auto& neighbor : neighbors) {
            if (seen.insert(neighbor)) {
                q.push_back(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
constexpr std::vector<std::vector<T>> GetAllFloodFillGroups(std::vector<T> all, auto neighborFunc) {
    std::vector<std::vector<T>> result;
    auto n = [&](const T& t) {
        auto neighbors = neighborFunc(t);
        return Constexpr::Within(neighbors, all);
        //std::vector<T> toKeep;
        //std::copy_if(neighbors.begin(), neighbors.end(), std::back_inserter(toKeep), [&](const T& neighbor) {
        //    return std::find(all.begin(), all.end(), neighbor) != all.end();
        //    });
        //return toKeep;
    };

    while (!all.empty()) {
        auto pos = all.back();
        all.pop_back();
        auto group = FloodFill(pos, n);
        result.push_back(group);
        all = Constexpr::Without(all, group);
    }

    return result;
}