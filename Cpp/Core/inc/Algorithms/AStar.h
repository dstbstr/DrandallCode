#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <queue>

#include "Constexpr/ConstexprGeometry.h"
#include "Constexpr/ConstexprCollections.h"

namespace AStarPrivate {
    template<typename T>
    struct MinimalPath {
        size_t Known = 10000;
        size_t Forcast = 10000;
        T Val{};

        constexpr MinimalPath() {}
        constexpr explicit MinimalPath(T t) : Val(t) {}

        constexpr bool operator<(const MinimalPath& other) const {
            return other.Forcast < Forcast;
        }
    };

    template<typename T>
    struct MaximalPath {
        size_t Known = 10000;
        size_t Forcast = 10000;
        T Val{};

        constexpr MaximalPath() {}
        constexpr explicit MaximalPath(T t) : Val(t) {}

        constexpr bool operator<(const MaximalPath& other) const {
            return Forcast < other.Forcast;
        }
    };

    template<typename T, typename State>
    constexpr std::vector<T> AStar(T start, auto costFunc, auto doneFunc, auto hFunc, auto nFunc) {
        Constexpr::Set<T> seen{};
        Constexpr::SmallMap<T, T> cameFrom{};
        Constexpr::SmallMap<T, State> state{};
        Constexpr::PriorityQueue<State> queue{};

        auto startState = State(start);
        startState.Known = 0;
        startState.Forcast = hFunc(start);
        state[start] = startState;
        //state.insert({ start, startState });
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

            if (doneFunc(current.Val)) {
                return constructPath(start, current.Val);
            }

            seen.insert(current.Val);
            for (auto neighbor : nFunc(current.Val)) {
                if (seen.contains(neighbor)) continue;

                auto known = current.Known + costFunc(current.Val, neighbor);
                if (known < state[neighbor].Known) {
                    cameFrom[neighbor] = current.Val;
                    state[neighbor].Known = known;
                    state[neighbor].Forcast = known + hFunc(neighbor);
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

template<typename T>
constexpr std::vector<T> AStarMin(T start,
    auto costFunc,
    auto doneFunc,
    auto hFunc,
    auto nFunc) {

    return AStarPrivate::AStar<T, AStarPrivate::MinimalPath<T>>(start, costFunc, doneFunc, hFunc, nFunc);
}

template<typename T>
constexpr std::vector<T> AStarMin(T start, T end, auto nFunc) {
    auto costFunc = [](const T&, const T&) {return 1; };
    auto isComplete = [&end](const T& pos) { return pos == end; };
    auto h = [&end](const T& pos) { return static_cast<size_t>(MDistance(pos, end)); };

    return AStarPrivate::AStar<T, AStarPrivate::MinimalPath<T>>(start, costFunc, isComplete, h, nFunc);
}

template<typename T>
constexpr std::vector<T> AStarMax(T start,
    auto costFunc,
    auto doneFunc,
    auto hFunc,
    auto nFunc) {

    return AStarPrivate::AStar<T, AStarPrivate::MaximalPath<T>>(start, costFunc, doneFunc, hFunc, nFunc);
}

template<typename T>
constexpr std::vector<T> AStarMax(T start, T end, auto nFunc) {
    auto costFunc = [](const T&, const T&) {return 1; };
    auto isComplete = [&end](const T& pos) { return pos == end; };
    auto h = [&end](const T& pos) { return static_cast<size_t>(MDistance(pos, end)); };

    return AStarPrivate::AStar<T, AStarPrivate::MaximalPath<T>>(start, costFunc, isComplete, h, nFunc);
}