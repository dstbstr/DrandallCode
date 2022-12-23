#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>

#include "Platform/Types.h"

template<typename T, class THash = std::hash<T>>
std::vector<T> AStar(T start,
    std::function<u32(const T&, const T&)> costFunc, 
    std::function<bool(u32, u32)> compareFunc, 
    std::function<bool(const T&)> isCompleteFunc,
    std::function<u32(const T&)> heuristicFunc, 
    std::function<std::vector<T>(const T&)> neighborFunc) {

    std::unordered_set<T, THash> seen{};
    std::vector<T> queue{};
    std::unordered_map<T, T, THash> cameFrom{};

    struct State {
        u32 gScore = 10000;
        u32 fScore = 10000;
    };
    std::unordered_map<T, State, THash> state;
    cameFrom[start] = start;
    state[start].gScore = 0;
    state[start].fScore = heuristicFunc(start);

    queue.push_back(start);

    auto sortFunc = [&compareFunc, &state](const T& lhs, const T& rhs) { 
        return !compareFunc(state[lhs].fScore, state[rhs].fScore); //reverse compare because we pop from the back
    };
    auto constructPath = [&cameFrom](const T& start, const T& end) {
        std::vector<T> result{ end };
        auto current = end;
        while (current != start) {
            auto next = cameFrom[current];
            result.push_back(next);
            current = next;
        }

        std::reverse(result.begin(), result.end());
        return result;
    };

    while (!queue.empty()) {
        std::sort(queue.begin(), queue.end(), sortFunc);
        auto current = queue.back();
        queue.pop_back();

        if (isCompleteFunc(current)) {
            return constructPath(start, current);
        }

        seen.insert(current);
        for (auto neighbor : neighborFunc(current)) {
            if (seen.find(neighbor) != seen.end()) continue;
            auto guess = state[current].gScore + costFunc(current, neighbor);
            if (guess < state[neighbor].gScore) {
                cameFrom[neighbor] = current;
                state[neighbor].gScore = guess;
                state[neighbor].fScore = guess + heuristicFunc(neighbor);
                if (std::find(queue.cbegin(), queue.cend(), neighbor) == queue.cend()) {
                    queue.push_back(neighbor);
                }
            }
        }
    }

    return {};
}
