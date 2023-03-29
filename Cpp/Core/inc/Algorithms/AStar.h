#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <queue>

#include "Constexpr/ConstexprGeometry.h"

namespace AStarPrivate {
    template<typename T>
    struct MinimalPath {
        size_t Known = 10000;
        size_t Forcast = 10000;
        T Val;

        MinimalPath() {}
        explicit MinimalPath(T t) : Val(t) {}

        constexpr bool operator<(const MinimalPath& other) const {
            return other.Forcast < Forcast;
        }
    };

    template<typename T>
    struct MaximalPath {
        size_t Known = 10000;
        size_t Forcast = 10000;
        T Val;

        MaximalPath() {}
        explicit MaximalPath(T t) : Val(t) {}

        constexpr bool operator<(const MaximalPath& other) const {
            return Forcast < other.Forcast;
        }
    };

    template<typename T, class THash, typename State>
    std::vector<T> AStar(T start,
        std::function<size_t(const T&, const T&)> costFunc,
        std::function<bool(const T&)> isCompleteFunc,
        std::function<size_t(const T&)> heuristicFunc,
        std::function<std::vector<T>(const T&)> neighborFunc) {

        std::unordered_set<T, THash> seen{};
        std::unordered_map<T, T, THash> cameFrom{};
        std::priority_queue<State> queue{};
        std::unordered_map<T, State, THash> state;

        auto startState = State(start);
        startState.Known = 0;
        startState.Forcast = heuristicFunc(start);
        state.insert({ start, startState });
        cameFrom[start] = start;
        queue.push(startState);

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
            auto current = queue.top();
            queue.pop();

            if (isCompleteFunc(current.Val)) {
                return constructPath(start, current.Val);
            }

            seen.insert(current.Val);
            for (auto neighbor : neighborFunc(current.Val)) {
                if (seen.find(neighbor) != seen.end()) continue;

                auto known = current.Known + costFunc(current.Val, neighbor);
                if (known < state[neighbor].Known) {
                    cameFrom[neighbor] = current.Val;
                    state[neighbor].Known = known;
                    state[neighbor].Forcast = known + heuristicFunc(neighbor);
                    State next = State(neighbor);
                    next.Known = known;
                    next.Forcast = state[neighbor].Forcast;
                    queue.push(next);
                }
            }
        }

        return {};
    }
}

template<typename T, class THash = std::hash<T>>
std::vector<T> AStarMin(T start,
    std::function<size_t(const T&, const T&)> costFunc,
    std::function<bool(const T&)> isCompleteFunc,
    std::function<size_t(const T&)> heuristicFunc,
    std::function<std::vector<T>(const T&)> neighborFunc) {
    
    return AStarPrivate::AStar<T, THash, AStarPrivate::MinimalPath<T>>(start, costFunc, isCompleteFunc, heuristicFunc, neighborFunc);
}

template<typename T, class THash = std::hash<T>>
std::vector<T> AStarMin(T start, T end, std::function<std::vector<T>(const T&)> neighborFunc) {
    auto costFunc = [](const T&, const T&) {return 1; };
    auto isComplete = [&end](const T& pos) { return pos == end; };
    auto h = [&end](const T& pos) { return static_cast<size_t>(MDistance(pos, end)); };

    return AStarPrivate::AStar<T, THash, AStarPrivate::MinimalPath<T>>(start, costFunc, isComplete, h, neighborFunc);
}

template<typename T, class THash = std::hash<T>>
std::vector<T> AStarMax(T start,
    std::function<size_t(const T&, const T&)> costFunc,
    std::function<bool(const T&)> isCompleteFunc,
    std::function<size_t(const T&)> heuristicFunc,
    std::function<std::vector<T>(const T&)> neighborFunc) {

    return AStarPrivate::AStar<T, THash, AStarPrivate::MaximalPath<T>>(start, costFunc, isCompleteFunc, heuristicFunc, neighborFunc);
}

template<typename T, class THash = std::hash<T>>
std::vector<T> AStarMax(T start, T end, std::function<std::vector<T>(const T&)> neighborFunc) {
    auto costFunc = [](const T&, const T&) {return 1; };
    auto isComplete = [&end](const T& pos) { return pos == end; };
    auto h = [&end](const T& pos) { return static_cast<size_t>(MDistance(pos, end)); };

    return AStarPrivate::AStar<T, THash, AStarPrivate::MaximalPath<T>>(start, costFunc, isComplete, h, neighborFunc);
}
