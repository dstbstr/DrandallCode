#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_set>

template<typename T, class THash = std::hash<T>>
constexpr void FloodFill(T start, std::vector<T>& output, std::function<std::vector<T>(T)> NeighborFunc) {
    std::unordered_set<T, THash> seen{ start };
    std::vector<T> q{ start };

    while (!q.empty()) {
        auto current = q.back();
        q.pop_back();
        output.push_back(current);

        auto neighbors = NeighborFunc(current);
        for (const auto& neighbor : neighbors) {
            if (seen.find(neighbor) == seen.end()) {
                seen.insert(neighbor);
                q.push_back(neighbor);
            }
        }
    }
}