#pragma once

#include <vector>
#include <array>

/*
Given a graph with N nodes (A through E in this example)
Represent the distances between Node A and B in a table

  A B C D E
A 0
B   0
C     0
D       0
E         0

Fill in the known distances (so if B to C is 3, then graph[B][C] = 3)
Fill in all unknowns with infinity
This will then find the shortest path from each node to every other node
*/
template<size_t Verts, typename T>
constexpr void FloydWarshall(std::array<std::array<T, Verts>, Verts>& table) {
    for (size_t i = 0; i < Verts; i++) {
        for (size_t x = 0; x < Verts; x++) {
            for (size_t y = 0; y < Verts; y++) {
                table[x][y] = std::min<T>(table[x][y], table[x][i] + table[i][y]);
            }
        }
    }
}

template<typename T>
constexpr void FloydWarshall(std::vector<std::vector<T>>& table) {
    for (size_t i = 0; i < table.size(); i++) {
        for (size_t x = 0; x < table.size(); x++) {
            for (size_t y = 0; y < table[0].size(); y++) {
                table[x][y] = std::min(table[x][y], table[x][i] + table[i][y]);
            }
        }
    }
}

namespace ConstexprTests {
    constexpr bool TestFloydWarshall(size_t x, size_t y, size_t expectedValue) {
        std::array<std::array<size_t, 4>, 4> graph = { {
            { 0, 5, 99, 10 },
            { 99, 0, 3, 99 },
            { 99, 99, 0, 1} ,
            { 99, 99, 99, 0 }
        } };

        FloydWarshall(graph);
        return graph[y][x] == expectedValue;
    }

    static_assert(TestFloydWarshall(0, 0, 0));
    static_assert(TestFloydWarshall(1, 0, 5));
    static_assert(TestFloydWarshall(2, 0, 8));
    static_assert(TestFloydWarshall(3, 1, 4));
}
