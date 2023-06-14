#pragma once

#include "2015/d18_LightWar2.h"
SOLUTION(2015, 18) {
    //# == on
    //. == off
    //nothing == off

    template<size_t N, size_t M>
    constexpr std::array<std::array<bool, N>, M> BuildArray(const std::string_view & input) {
        auto result = std::array<std::array<bool, N>, M>{};
        if (input.size() != N * M) {
            throw std::logic_error("Bad input");
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                result[i][j] = input[i * N + j] == '#';
            }
        }

        return result;
    }

    template<size_t N, size_t M>
    constexpr u32 CountLitNeighbors(const std::array<std::array<bool, N>, M> input, size_t n, size_t m) {
        u32 lit = 0;
        if (n > 0) {
            if (input[n - 1][m]) lit++; //left
            if (m > 0 && input[n - 1][m - 1]) lit++; //up left
            if (m < M - 1 && input[n - 1][m + 1]) lit++; // up right
        }
        if (m > 0 && input[n][m - 1]) lit++; // up

        if (n < N - 1) {
            if (input[n + 1][m]) lit++; // right
            if (m < M - 1 && input[n + 1][m + 1]) lit++; //down right
            if (m > 0 && input[n + 1][m - 1]) lit++; //down left
        }
        if (m < M - 1 && input[n][m + 1]) lit++; //down

        return lit;
    }

    /*
    template<size_t N, size_t M>
    constexpr std::string ToString(const std::array<std::array<bool, N>, M>&arr) {
        std::string result;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                result += (arr[i][j] ? '#' : '.');
            }
        }

        return result;
    }
    */

    //on with 2 or 3 on neighbors stays on
    //off with 3 on neightbors turns on

    template<size_t N, size_t M>
    constexpr void Next(std::array<std::array<bool, N>, M>&in) {
        auto temp = std::array<std::array<bool, N>, M>{};

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                auto litNeighbors = CountLitNeighbors(in, i, j);
                if (in[i][j]) {
                    temp[i][j] = litNeighbors == 2 || litNeighbors == 3;
                }
                else {
                    temp[i][j] = litNeighbors == 3;
                }
            }
        }

        memcpy(&in, &temp, N * M);
    }

    template<size_t N, size_t M>
    constexpr void SetCorners(std::array<std::array<bool, N>, M>& in) {
        in[0][0] = true;
        in[0].back() = true;
        in.back()[0] = true;
        in.back().back() = true;
    }

    template<size_t N, size_t M>
    constexpr u32 CountLit(const std::array<std::array<bool, N>, M>&in) {
        u32 result = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (in[i][j]) result++;
            }
        }

        return result;
    }

    PART_ONE() {
        auto lights = BuildArray<100, 100>(Constexpr::JoinVec("", lines));
        for (auto i = 0; i < 100; i++) {
            Next(lights);
        }

        return Constexpr::ToString(CountLit(lights));
    }
    PART_TWO() {
        auto lights = BuildArray<100, 100>(Constexpr::JoinVec("", lines));
        SetCorners(lights);

        for (auto i = 0; i < 100; i++) {
            Next(lights);
            SetCorners(lights);
        }

        return Constexpr::ToString(CountLit(lights));
    }
    TESTS() {
        static_assert(BuildArray<6, 6>(
            ".#.#.#"
            "...##."
            "#....#"
            "..#..."
            "#.#..#"
            "####..")[2][0] == true);
 
        static_assert(CountLitNeighbors(BuildArray<6, 6>(
            ".#.#.#"
            "...##."
            "#....#"
            "..#..."
            "#.#..#"
            "####.."), 0, 0) == 1);

        static_assert(CountLitNeighbors(BuildArray<6, 6>(
            ".#.#.#"
            "...##."
            "#....#"
            "..#..."
            "#.#..#"
            "####.."), 1, 4) == 4);

        static_assert(CountLit(BuildArray<6, 6>(
            ".#.#.#"
            "...##."
            "#....#"
            "..#..."
            "#.#..#"
            "####..")) == 15);

        return true;
    }
}