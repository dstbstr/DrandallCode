#pragma once

#include "Common.h"

enum struct State {Floor, Empty, Occupied};

using Grid = std::vector<std::vector<State>>;
constexpr Grid ParseInput(const std::vector<std::string>& lines) {
    Grid result;

    for (const auto& line : lines) {
        std::vector<State> row;
        for (auto c : line) {
            if (c == '.') row.push_back(State::Floor);
            else if (c == 'L') row.push_back(State::Empty);
            else if (c == '#') row.push_back(State::Occupied);
            else throw "Unknown Value";
        }
        result.push_back(row);
    }

    return result;
}

constexpr State NextState(State current, const std::vector<State>& neighbors, s32 neighborLimit) {
    if (current == State::Empty) {
        if (std::all_of(neighbors.begin(), neighbors.end(), [](State state) { return state != State::Occupied; })) {
            return State::Occupied;
        }
    }
    else if (current == State::Occupied) {
        if (std::count_if(neighbors.begin(), neighbors.end(), [](State state) { return state == State::Occupied; }) >= neighborLimit) {
            return State::Empty;
        }
    }

    return current;
}

constexpr Grid Next(const Grid& current) {
    Grid result = current;
    RowCol limits = { current.size() - 1, current[0].size() - 1 };

    for (size_t row = 0; row < current.size(); row++) {
        for (size_t col = 0; col < current[row].size(); col++) {
            auto currentState = current[row][col];
            if (currentState == State::Floor) {
                result[row][col] = currentState;
                continue;
            }

            RowCol pos = { row, col };
            auto neighbors = GetAllNeighbors(pos, limits);
            std::vector<State> neighborStates;
            for (const auto& neighbor : neighbors) {
                neighborStates.push_back(current[neighbor.Row][neighbor.Col]);
            }
            result[row][col] = NextState(currentState, neighborStates, 4);
        }
    }
    return result;
}

auto PartOne(const std::vector<std::string>& lines) {
    auto grid = ParseInput(lines);
    while (true) {
        auto next = Next(grid);
        if (next == grid) break;
        else grid = next;
    }

    size_t result = 0;
    for (const auto& row : grid) {
        for (const auto& col : row) {
            result += col == State::Occupied;
        }
    }

    return result;
}

constexpr bool CanFindSeatInDirection(const Grid& grid, RowCol pos, s32 dX, s32 dY, RowCol limits, State& outState) {
    pos.Row += dY;
    pos.Col += dX;
    while (pos.Row >= 0 && pos.Row <= limits.Row && pos.Col >= 0 && pos.Col <= limits.Col) {
        if (grid[pos.Row][pos.Col] != State::Floor) {
            outState = grid[pos.Row][pos.Col];
            return true;
        }
        pos.Row += dY;
        pos.Col += dX;
    }
    return false;
}

constexpr std::vector<State> FindNearest8Seats(const Grid& grid, RowCol pos, RowCol limits) {
    std::vector<State> result;
    std::vector<Coord> deltas = { {0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (const auto& delta : deltas) {
        State neighborState;
        if (CanFindSeatInDirection(grid, pos, delta.X, delta.Y, limits, neighborState)) result.push_back(neighborState);
    }
    
    return result;
}

constexpr Grid Next2(const Grid& current) {
    Grid result = current;
    RowCol limits = { current.size() - 1, current[0].size() - 1 };

    for (size_t row = 0; row < current.size(); row++) {
        for (size_t col = 0; col < current[row].size(); col++) {
            auto currentState = current[row][col];
            if (currentState == State::Floor) {
                result[row][col] = currentState;
                continue;
            }

            RowCol pos = { row, col };
            auto neighbors = FindNearest8Seats(current, pos, limits);
            result[row][col] = NextState(currentState, neighbors, 5);
        }
    }
    return result;
}

auto PartTwo(const std::vector<std::string>& lines) {
    auto grid = ParseInput(lines);
    while (true) {
        auto next = Next2(grid);
        if (next == grid) break;
        else grid = next;
    }

    size_t result = 0;
    for (const auto& row : grid) {
        for (const auto& col : row) {
            result += col == State::Occupied;
        }
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines));
    return ToString(PartTwo(lines));
}

bool RunTests() {
    std::vector<std::string> lines = {
        "L.LL.LL.LL",
        "LLLLLLL.LL",
        "L.L.L..L..",
        "LLLL.LL.LL",
        "L.LL.LL.LL",
        "L.LLLLL.LL",
        "..L.L.....",
        "LLLLLLLLLL",
        "L.LLLLLL.L",
        "L.LLLLL.LL"
    };

    if (PartOne(lines) != 37) return false;
    if (PartTwo(lines) != 26) return false;
    return true;
}