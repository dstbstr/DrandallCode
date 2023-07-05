#include "2022/d12_HillClimb.h"

SOLUTION(2022, 12) {
    template<size_t Rows, size_t Cols>
    constexpr std::array<std::array<char, Cols>, Rows> BuildMap(const std::vector<std::string>&lines, size_t & startIndex, size_t & endIndex) {
        std::array<std::array<char, Cols>, Rows> result{};

        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                result[row][col] = lines[row][col];
                if (lines[row][col] == 'S') {
                    result[row][col] = 'a';
                    startIndex = Constexpr::GetIndex<Rows, Cols>(row, col);
                }
                else if (lines[row][col] == 'E') {
                    result[row][col] = 'z';
                    endIndex = Constexpr::GetIndex<Rows, Cols>(row, col);
                }
            }
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr std::vector<size_t> FindAllStarts(const std::array<std::array<char, Cols>, Rows>&map) {
        std::vector<size_t> result;

        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                if (map[row][col] == 'a') {
                    result.push_back(Constexpr::GetIndex<Rows, Cols>(row, col));
                }
            }
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr std::vector<size_t> FindNext(size_t currentIndex) {
        size_t row, col;
        Constexpr::GetRowCol<Rows, Cols>(currentIndex, row, col);

        std::vector<size_t> result{};
        if (row > 0) {
            result.push_back(Constexpr::GetIndex<Rows, Cols>(row - 1, col));
        }
        if (col > 0) {
            result.push_back(Constexpr::GetIndex<Rows, Cols>(row, col - 1));
        }
        if (row < Rows - 1) {
            result.push_back(Constexpr::GetIndex<Rows, Cols>(row + 1, col));
        }
        if (col < Cols - 1) {
            result.push_back(Constexpr::GetIndex<Rows, Cols>(row, col + 1));
        }

        return result;
    }

    static_assert(FindNext<3, 3>(0).size() == 2);
    static_assert(FindNext<3, 3>(4).size() == 4);

#include <iostream>

    template<size_t Rows, size_t Cols>
    void Print(const std::array<std::array<char, Cols>, Rows>&map, const std::vector<size_t>&current) {
        size_t row, col;
        for (auto index : current) {
            GetRowCol<Rows, Cols>(index, row, col);
            std::cout << "map[" << row << "][" << col << "] = " << map[row][col] << "\n";
        }

        std::cout << "\n";
    }

    template<size_t Rows, size_t Cols>
    constexpr u32 Bfs(const std::array<std::array<char, Cols>, Rows>&map, size_t startIndex, size_t endIndex) {
        std::unordered_set<size_t> visited{ startIndex };
        size_t row, col;
        Constexpr::GetRowCol<Rows, Cols>(startIndex, row, col);

        std::vector<size_t> current{startIndex};
        std::unordered_set<size_t> next{};

        u32 steps = 0;
        while (true) {
            for (auto index : current) {
                if (index == endIndex) return steps;

                Constexpr::GetRowCol<Rows, Cols>(index, row, col);
                auto height = map[row][col];

                auto moves = FindNext<Rows, Cols>(index);
                for (auto move : moves) {
                    Constexpr::GetRowCol<Rows, Cols>(move, row, col);
                    if (map[row][col] <= static_cast<char>(static_cast<u8>(height) + 1)) {
                        next.insert(move);
                    }
                }
            }

            current.clear();
            std::copy_if(next.cbegin(), next.cend(), std::back_inserter(current), [&visited](const size_t index) {
                return visited.find(index) == visited.end();
                });

            visited.insert(current.cbegin(), current.cend());
            next.clear();
            if (current.empty()) {
                break; //Fail case
            }

            steps++;
        }
        return 999; //fail case
    }

    std::string RunFirst(const std::vector<std::string>&lines) {
        size_t startIndex, endIndex;
        auto map = BuildMap<41, 66>(lines, startIndex, endIndex);
        return Constexpr::ToString(Bfs<41, 66>(map, startIndex, endIndex));
    }

    std::string Run(const std::vector<std::string>&lines) {
        size_t startIndex, endIndex;
        auto map = BuildMap<41, 66>(lines, startIndex, endIndex);
        auto allStarts = FindAllStarts(map);

        u32 best = 999;
        for (auto start : allStarts) {
            auto length = Bfs<41, 66>(map, start, endIndex);
            best = std::min(best, length);
        }

        return Constexpr::ToString(best);
    }

    bool RunTests() {
        std::vector<std::string> lines{
            "Sabqponm",
                "abcryxxl",
                "accszExk",
                "acctuvwj",
                "abdefghi"
        };
        size_t startIndex, endIndex;
        auto map = BuildMap<5, 8>(lines, startIndex, endIndex);
        if (startIndex != 0) return false;
        if (endIndex != 21) return false;

        size_t row, col;
        Constexpr::GetRowCol<5, 8>(startIndex, row, col);
        if (map[row][col] != 'a') return false;

        Constexpr::GetRowCol<5, 8>(endIndex, row, col);
        if (map[row][col] != 'z') return false;

        if (Bfs<5, 8>(map, startIndex, endIndex) != 31) return false;

        auto allStarts = FindAllStarts(map);
        u32 best = 999;
        for (auto start : allStarts) {
            auto length = Bfs<5, 8>(map, start, endIndex);
            best = std::min(best, length);
        }

        if (best != 29) return false;


        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}